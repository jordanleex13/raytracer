/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/

#include <cassert>

#include "raytracer.h"

#define EPSILON 0.0001
#define SHADOW_INTENSITY 1.0 // set to 1.0 for default


//#define MULTITHREADING // Toggle option for multithreading

#ifdef MULTITHREADING
#include <omp.h>
#define parallelism_enabled 1
#else
#define parallelism_enabled 0
#endif

/***** FEATURES *****/

#define DEPTH 3 // number of bounces before ray dies

// #define ANTI_ALIASING
// #define NUM_ANTIALIASING_RAY 3

//#define DOF
//#define NUM_RAND_DOF_RAY 3
//
// #define SHADOWING
// #define SOFT_SHADOWS
//
// #define REFLECTION
// #define GLOSSY
//
//#define REFRACTION

void Raytracer::traverseScene(Scene &scene, Ray3D &ray) {
    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode *node = scene[i];

        if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
            ray.intersection.mat = node->mat;
        }
    }
}

void Raytracer::computeTransforms(Scene &scene) {
    // right now this method might seem redundant. But if you decide to implement
    // scene graph this is where you would propagate transformations to child nodes

    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode *node = scene[i];

        node->modelToWorld = node->trans;
        node->worldToModel = node->invtrans;
    }
}

void Raytracer::computeShading(Ray3D &ray, Scene &scene, LightList &light_list) {
    for (size_t i = 0; i < light_list.size(); ++i) {
        LightSource *light = light_list[i];
        light->shade(ray);

#ifdef SHADOWING
        // shoot a ray in the reverse light direction
        Point3D lightPos = light->get_position();

        // sample from random unit sphere to get soft shadow effect. Less computational than having a bunch of point lights
#ifdef SOFT_SHADOWS
        double r = 0.9 * generateRandom();
        double theta = 2 * M_PI * generateRandom();
        double phi = 2 * M_PI * generateRandom();

        // randomly displace the position of the light to simulate a 'sphere light'
        lightPos = lightPos + Vector3D(r * cos(theta) * sin(phi), r * sin(theta) * sin(phi), r * cos(phi));
#endif

        // get vector going in opposite direction
        Point3D intersection = ray.intersection.point;
        Vector3D reverseLightVect = lightPos - intersection;
        double distToLight = reverseLightVect.length();
        reverseLightVect.normalize();

        Point3D startPos = intersection + EPSILON * reverseLightVect;
        Ray3D reverseRay(startPos, reverseLightVect);
        traverseScene(scene, reverseRay);
        // if the reverse ray hit any other object, it's shadow
        if (!reverseRay.intersection.none) {
            if (reverseRay.intersection.t_value <= distToLight) {
                double transmittance = reverseRay.intersection.mat->transmittance;
                if (!transmittance) {
                    ray.col = (1.0 - SHADOW_INTENSITY) * ray.col + SHADOW_INTENSITY * ray.intersection.mat->ambient;    // overwrites ray colour
                } else {
                    ray.col = transmittance * ray.col + (1 - transmittance) * ray.intersection.mat->ambient;
                }
                ray.col.clamp();
            }
        }
#endif
    }
}

Ray3D Raytracer::getReflectedRay(Ray3D &ray) {

    Vector3D incident = -ray.dir;
    incident.normalize();
    Vector3D N = ray.intersection.normal;
    N.normalize();

    // Note this is not R in Phong Shading
    // This is the eye to intersection ray that is reflected (nothing to do with light src)
    Vector3D R = 2 * (incident.dot(N)) * N - incident; // ray.dir - 2*( normal.dot(ray.dir) * normal );
    R.normalize();

    Ray3D reflectedRay(ray.intersection.point + EPSILON * R, R);

#ifdef GLOSSY
    // Glossy reflection simulates 'roughness'
    Vector3D u = R.cross(N);
    u.normalize();
    Vector3D v = R.cross(u);
    v.normalize();

    // Predict roughness value using alpha (specular exponent)
    // http://www.raytracegroundup.com/downloads/Chapter25.pdf
    // find theta using formula. generate a random density around hemisphere
    double theta = acos(pow(generateRandom(), 1.0 / (ray.intersection.mat->specular_exp)));
    double phi = M_PI_TIMES_2 * generateRandom();
    double x = sin(theta) * cos(phi);
    double y = sin(theta) * sin(phi);
    double z = cos(theta);

    // direction
    reflectedRay.dir = x * u + y * v + z * R;
    reflectedRay.dir.normalize();

    // origin is slightly offset
    reflectedRay.origin = ray.intersection.point + EPSILON * reflectedRay.dir;
#endif
    return reflectedRay;
}

bool Raytracer::getRefractedRay(Ray3D &ray, Ray3D &refractedRay, double &T) {

    Vector3D I = -ray.dir;
    I.normalize();
    Vector3D N = ray.intersection.normal;
    N.normalize();

    // refraction coefficient of air and material
    double n_air = 1.0;
    double n_mat = ray.intersection.mat->n_refr;

    if (n_mat == 0) { return false; }

    double cos_in = I.dot(N);
    // double theta_in = acos(cos_in);
    double n_in, n_out;
    if (cos_in < 0.0) {
        // ray is shooting from the object to the air
        cos_in = -cos_in;
        n_in = n_mat;
        n_out = n_air;
   	    // theta_in = 2.0*M_PI - theta_in;
    } else {
        N = -N;
        // ray is shooting from the air to the object
        n_in = n_air;
        n_out = n_mat;
    }

    double ratio_n = n_in / n_out;
    double k = 1.0 - ratio_n * ratio_n * (1.0 - cos_in * cos_in);
    if (k < 0) { return false; } // total internal reflection, no refraction
    double cos_out = sqrt(k);
    Vector3D Rr = -ratio_n * I + -(ratio_n * cos_in - cos_out) * N;
    Rr.normalize();

    // http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
    double R0 = pow((n_in - n_out) / (n_in + n_out), 2.0);
    // Reflectance.
    double R = R0 + (1.0 - R0) * (1.0 - cos_in);
    // Transmittance.
    T = 1.0 - R;

    // initialize passed in ray
    refractedRay.origin = ray.intersection.point + EPSILON * Rr;
    refractedRay.dir = Rr;
    refractedRay.dir.normalize();

    return true;
}

Color Raytracer::shadeRay(Ray3D &ray, Scene &scene, LightList &light_list, int depth) {
    Color col(0.0, 0.0, 0.0);
    if (depth == 0)
        return col;

    traverseScene(scene, ray);

    // Don't bother shading if the ray didn't hit anything.
    if (!ray.intersection.none) {
        computeShading(ray, scene, light_list);
        col = ray.col;

#ifdef REFLECTION
        Color &spec = ray.intersection.mat->specular;
        Ray3D reflectedRay = getReflectedRay(ray);

        Color reflectedColor = shadeRay(reflectedRay, scene, light_list, depth - 1);
#ifndef REFRACTION
        col = col + (spec * reflectedColor);
#endif
#endif

#ifdef REFRACTION
        Ray3D refractedRay;
        double T;
        double Ks = ray.intersection.mat->Ks;
        double transmittance = ray.intersection.mat->transmittance;
        if (getRefractedRay(ray, refractedRay, T)) {
            Color refractedColor = shadeRay(refractedRay, scene, light_list, depth - 1);
#ifndef REFLECTION
            col = (1.0 - transmittance)*col + transmittance * refractedColor;
#else
            col = (1.0 - Ks) * col + Ks * ((1.0 - T) * (spec * reflectedColor) + T * refractedColor);
#endif
        } else {
#ifdef REFLECTION
            col = col + (spec * reflectedColor);
#endif
        }
#endif

    }
    col.clamp();
    return col;
}

void Raytracer::render(Camera &camera, Scene &scene, LightList &light_list, Image &image) {
    computeTransforms(scene);

    Matrix4x4 viewToWorld;
    assert(tan(camera.fov * M_PI / 360.0) != 0);
    double factor = (double(image.height) / 2) / tan(camera.fov * M_PI / 360.0);

    viewToWorld = camera.initInvViewMatrix();

    Point3D origin(0, 0, 0);

#pragma omp parallel for if(parallelism_enabled)
    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
            Point3D imagePlane;
            imagePlane[2] = -1;
            Point3D rand_on_lens;
            rand_on_lens[2] = 0;
            Color col;
#ifndef ANTI_ALIASING
            imagePlane[0] = (-double(image.width) / 2 + 0.5 + j) / factor;
            imagePlane[1] = (-double(image.height) / 2 + 0.5 + i) / factor;
            Ray3D ray;
            ray.origin = origin;
            ray.dir = imagePlane - ray.origin;

#ifdef DOF
            Point3D focal = ray.origin + camera.focalLength*ray.dir;
            for(int a = 0; a < NUM_RAND_DOF_RAY; a++){
                for(int b = 0; b < NUM_RAND_DOF_RAY; b++){
                    rand_on_lens[0] = origin[0] + camera.aperture/NUM_RAND_DOF_RAY*(a+0.5);
                    rand_on_lens[1] = origin[1] + camera.aperture/NUM_RAND_DOF_RAY*(b+0.5);

                    // create ray in view space (camera space)
                    Ray3D ray;
                    ray.origin = rand_on_lens;
                    ray.dir = focal - ray.origin;

                    // convert ray to world space
                    ray.origin = viewToWorld * ray.origin;
                    ray.dir = viewToWorld * ray.dir;

                    Color subcol = shadeRay(ray, scene, light_list, 1);
                    col[0] += subcol[0]/NUM_RAND_DOF_RAY/NUM_RAND_DOF_RAY;
                    col[1] += subcol[1]/NUM_RAND_DOF_RAY/NUM_RAND_DOF_RAY;
                    col[2] += subcol[2]/NUM_RAND_DOF_RAY/NUM_RAND_DOF_RAY;
                }
            }
#else //DOF not defined
            // convert ray to world space
            ray.origin = viewToWorld * ray.origin;
            ray.dir = viewToWorld * ray.dir;

            col = shadeRay(ray, scene, light_list, DEPTH);
#endif
#endif

#ifdef ANTI_ALIASING
            for (int m = 0; m < NUM_ANTIALIASING_RAY; m++) {
                for (int n = 0; n < NUM_ANTIALIASING_RAY; n++) {
                    imagePlane[0] = (-double(image.width) / 2 + j + 1.0 / NUM_ANTIALIASING_RAY * m +
                                     1.0 / NUM_ANTIALIASING_RAY / 2) / factor;
                    imagePlane[1] = (-double(image.height) / 2 + i + 1.0 / NUM_ANTIALIASING_RAY * n +
                                     1.0 / NUM_ANTIALIASING_RAY / 2) / factor;

                    // create ray in view space (camera space)
                    Ray3D ray;
                    ray.origin = origin;
                    ray.dir = imagePlane - ray.origin;

                    // convert ray to world space
                    ray.origin = viewToWorld * ray.origin;
                    ray.dir = viewToWorld * ray.dir;
#ifdef DOF
                    Point3D focal = ray.origin + camera.focalLength*ray.dir;
                    for(int a = 0; a < NUM_RAND_DOF_RAY; a++){
                        for(int b = 0; b < NUM_RAND_DOF_RAY; b++){
                            rand_on_lens[0] = origin[0] + camera.aperture/NUM_RAND_DOF_RAY*(a+0.5);
                            rand_on_lens[1] = origin[1] + camera.aperture/NUM_RAND_DOF_RAY*(b+0.5);

                            // create ray in view space (camera space)
                            Ray3D ray;
                            ray.origin = rand_on_lens;
                            ray.dir = focal - ray.origin;

                            // convert ray to world space
                            ray.origin = viewToWorld * ray.origin;
                            ray.dir = viewToWorld * ray.dir;

                            Color subcol = shadeRay(ray, scene, light_list, DEPTH);
                            col[0] += subcol[0]/NUM_RAND_DOF_RAY/NUM_RAND_DOF_RAY/NUM_ANTIALIASING_RAY/NUM_ANTIALIASING_RAY;
                            col[1] += subcol[1]/NUM_RAND_DOF_RAY/NUM_RAND_DOF_RAY/NUM_ANTIALIASING_RAY/NUM_ANTIALIASING_RAY;
                            col[2] += subcol[2]/NUM_RAND_DOF_RAY/NUM_RAND_DOF_RAY/NUM_ANTIALIASING_RAY/NUM_ANTIALIASING_RAY;
                        }
                    }
#else
                    // after converting the camera position and ray origin should be same
                    assert(ray.origin[0] == camera.eye[0] && ray.origin[1] == camera.eye[1] &&
                           ray.origin[2] == camera.eye[2]);

                    Color subcol = shadeRay(ray, scene, light_list, DEPTH);
                    col[0] += subcol[0] / NUM_ANTIALIASING_RAY / NUM_ANTIALIASING_RAY;
                    col[1] += subcol[1] / NUM_ANTIALIASING_RAY / NUM_ANTIALIASING_RAY;
                    col[2] += subcol[2] / NUM_ANTIALIASING_RAY / NUM_ANTIALIASING_RAY;
#endif 
                }
            }

#endif
            image.setColorAtPixel(i, j, col);
        }
    }
}

