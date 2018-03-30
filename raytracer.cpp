/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/

#include <cassert>

#include "raytracer.h"

#define NUM_ANTIALIASING_RAY 3
#define EPSILON 0.0001

// Toggle options on/off
//#define ANTI_ALIASING
//#define SHADOWING
//#define SOFT_SHADOWS
//#define GLOSSY
//#define REFRACTION
//#define REFLECTION


void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
}

void Raytracer::computeTransforms(Scene& scene) {
	// right now this method might seem redundant. But if you decide to implement 
	// scene graph this is where you would propagate transformations to child nodes
		
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		node->modelToWorld = node->trans;
		node->worldToModel = node->invtrans; 
	}
}

void Raytracer::computeShading(Ray3D& ray, Scene& scene, LightList& light_list) {
	for (size_t i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];
		light->shade(ray);

#ifdef SHADOWING
		// shoot a ray in the reverse light direction
        Point3D lightPos = light->get_position();

        // sample from random unit sphere to get soft shadow effect
#ifdef SOFT_SHADOWS
        double r = 0.9 * generateRandom();  // TODO play around with r value to get better looking soft shadows
        double theta = 2 * M_PI * generateRandom();
        double phi = 2 * M_PI * generateRandom();
        lightPos = lightPos + Vector3D(r*cos(theta)*sin(phi), r*sin(theta)*sin(phi), r*cos(phi));
#endif

		Point3D intersection = ray.intersection.point;
		Vector3D reverseLightVect = lightPos - intersection;
		double distToLight = reverseLightVect.length();
		reverseLightVect.normalize();

		Point3D startPos = intersection + EPSILON*reverseLightVect;
		Ray3D reverseRay(startPos, reverseLightVect);
		traverseScene(scene, reverseRay);
		// if the reverse ray hit any other object, it is a shadow
		if(!reverseRay.intersection.none){
			if(reverseRay.intersection.t_value <= distToLight) {
        		ray.col = ray.intersection.mat->ambient;    // overwrites ray colour
        		ray.col.clamp();
        	}
		}
#endif
	}
}

Ray3D Raytracer::getReflectedRay(Ray3D& ray) {

    Vector3D incident = -ray.dir; // -ray.dir so that the formula for R works
    incident.normalize();
    Vector3D N = ray.intersection.normal;
    N.normalize();

    // Note this is not R in Phong Shading
    // This is the eye to intersection ray that is reflected (nothing to do with light src)
    Vector3D R = 2*(incident.dot(N))*N - incident; // ray.dir - 2*( normal.dot(ray.dir) * normal );
    R.normalize();

    Ray3D reflectedRay(ray.intersection.point + EPSILON * R, R);

#ifdef GLOSSY
    // Glossy reflection (see tutorial slides)
    Vector3D u = R.cross(N);
    u.normalize();
    Vector3D v = R.cross(u);
    v.normalize();

    // when alpha -> infinity, it's shiny, not rough
    // when roughness -> 0, theta phi -> 0, thus xy -> 0, z->1
    double roughness = 2 * M_PI * (1.0 / (ray.intersection.mat->specular_exp + 1.0));
    double theta = roughness * generateRandom();
    double phi = roughness * generateRandom();
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

bool Raytracer::getRefractedRay(Ray3D& ray, Ray3D& refractedRay, float& transmittance) {

    Vector3D incident = -ray.dir; // -ray.dir so that the formula for R works
    incident.normalize();
    Vector3D N = ray.intersection.normal;
    N.normalize();

    // refraction coefficient of air and material
    float n_air = 1.0;
    float n_mat = ray.intersection.mat->n_refr;

    if (n_mat==0) { return false;}

    float cos_in = incident.dot(N);
    float theta_in = acos(cos_in);
    float n_in, n_out;
    if (cos_in < 0.0){
    	// ray is shooting from the object to the air
    	n_in = n_mat;
    	n_out = n_air;
    	theta_in = 2.0*M_PI - theta_in;
    } else {
    	// ray is shooting from the air to the object
    	n_in = n_air;
    	n_out = n_mat;
    }

    float ratio_n = n_in/n_out;
    float cos_out = sqrt(1.0 - ratio_n*ratio_n*(1.0 - cos_in*cos_in));
    Vector3D Rr = ratio_n*incident + (ratio_n*cos_in - cos_out)*N;

    // http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
    float R0 = pow((n_in - n_out)/(n_in + n_out), 2.0);
    // Reflectance.
    float R = R0 + (1.0 - R0)*(1.0 - cos_in);
    // Transmittance.
    transmittance = 1.0 - R;
	
	// std::cout << "Ray: (" << ray.dir[0] << ", " << ray.dir[1] << ", " << ray.dir[2] << ")" << std::endl; 
	// std::cout << "refraction: (" << Rr[0] << ", " << Rr[1] << ", " << Rr[2] << ")" << std::endl;
    // initialize passed in ray
    refractedRay.origin = ray.intersection.point;
    refractedRay.dir = Rr;
    refractedRay.dir.normalize();

    return true;
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int depth) {
    Color col(0.0, 0.0, 0.0);
    if (depth == 0)
        return col;

	traverseScene(scene, ray);

	// Don't bother shading if the ray didn't hit anything.
	if (!ray.intersection.none) {
        computeShading(ray, scene, light_list);
        col = ray.col;

#ifdef REFLECTION
        Color& spec = ray.intersection.mat->specular;
        Ray3D reflectedRay = getReflectedRay(ray);

        Color reflectedColor = shadeRay(reflectedRay, scene, light_list, depth - 1);
        col = col + (spec * reflectedColor); // no += operator for color

#endif

#ifdef REFRACTION 
        Ray3D refractedRay;
        float transmittance;
        if (getRefractedRay(ray, refractedRay, transmittance)){
            // TODO: add color by transmittance
            Color refractedColor = shadeRay(refractedRay, scene, light_list, depth - 1);
            col = col + transmittance*refractedColor;
        }
#endif

    }
    col.clamp();
	return col; 
}	

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

	viewToWorld = camera.initInvViewMatrix();

	Point3D origin(0, 0, 0);

	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {								
			// Sets up ray origin and direction in view space, 
			// image plane is at z = -1.
			Point3D imagePlane;
			imagePlane[2] = -1;
			Color col;
#ifndef ANTI_ALIASING
            imagePlane[0] = (-double(image.width)/2 + 0.5 + j)/factor;
			imagePlane[1] = (-double(image.height)/2 + 0.5 + i)/factor;
            Ray3D ray;
            ray.origin = origin;
            ray.dir = imagePlane - ray.origin;

            // convert ray to world space
            ray.origin = viewToWorld * ray.origin;
            ray.dir = viewToWorld * ray.dir;

            int depth = 5;  // number of bounces before ray dies
    		col = shadeRay(ray, scene, light_list, depth);
#endif

#ifdef ANTI_ALIASING
			for(int m = 0; m < NUM_ANTIALIASING_RAY; m++){
				for(int n = 0; n < NUM_ANTIALIASING_RAY; n++){
					imagePlane[0] = (-double(image.width)/2 + j + 1.0/NUM_ANTIALIASING_RAY * m + 1.0/NUM_ANTIALIASING_RAY/2)/factor;
					imagePlane[1] = (-double(image.height)/2 + i + 1.0/NUM_ANTIALIASING_RAY * n + 1.0/NUM_ANTIALIASING_RAY/2)/factor;
					
					// create ray in view space (camera space)
					Ray3D ray;
		            ray.origin = origin;
		            ray.dir = imagePlane - ray.origin;

		            // convert ray to world space
		            ray.origin = viewToWorld * ray.origin;
		            ray.dir = viewToWorld * ray.dir;

		            // after converting the camera position and ray origin should be same
					assert(ray.origin[0] == camera.eye[0] && ray.origin[1] == camera.eye[1] && ray.origin[2] == camera.eye[2]);

		            int depth = 5;  // number of bounces before ray dies
					Color subcol = shadeRay(ray, scene, light_list, depth);
					col[0] += subcol[0]/NUM_ANTIALIASING_RAY/NUM_ANTIALIASING_RAY;
					col[1] += subcol[1]/NUM_ANTIALIASING_RAY/NUM_ANTIALIASING_RAY;
					col[2] += subcol[2]/NUM_ANTIALIASING_RAY/NUM_ANTIALIASING_RAY;
				}
			}
#endif
			image.setColorAtPixel(i, j, col);
		}
	}
}

