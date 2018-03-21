/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <random>
#include <limits>

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

void Raytracer::computeShading(Ray3D& ray, LightList& light_list) {
	for (size_t  i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];
		
		// Each lightSource provides its own shading function.
		// Implement shadows here if needed.
		light->shade(ray);        
	}
}

void Raytracer::getReflectedRay(Ray3D& ray, Ray3D& reflectedRay) {

    Vector3D incident = -ray.dir; // -ray.dir so that the formula for R works
    incident.normalize();
    Vector3D N = ray.intersection.normal;
    N.normalize();

    // Note this is not R in Phong Shading
    // This is the eye to intersection ray that is reflected (nothing to do with light src)
    Vector3D R = 2*(incident.dot(N))*N - incident;
    R.normalize();

    // Glossy reflection (see tutorial slides)
    Vector3D u = R.cross(N);
    u.normalize();
    Vector3D v = R.cross(u);
    v.normalize();

    // TODO probably put this in the class itself to avoid reinitializaiton
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double roughness = 2 * M_PI * (1.0 - ray.intersection.mat->specular_exp); // change roughness if doesn't work
    // add randomness to reflection
    double a = dis(gen);
    double b = dis(gen);
//        std::cout << a << " " << b << std::endl;
    double theta = roughness * a;
    double phi = roughness * b;
    double x = sin(theta) * cos(phi);
    double y = sin(theta) * sin(phi);
    double z = cos(theta);

    // initialize passed in ray
    reflectedRay.origin = ray.intersection.point;
    reflectedRay.dir = x*u + y*v + z*R;
    reflectedRay.dir.normalize();
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int depth) {
    Color col(0.0, 0.0, 0.0);
    if (depth == 0)
        return col;

	traverseScene(scene, ray);

	// Don't bother shading if the ray didn't hit anything.
	if (!ray.intersection.none) {
        computeShading(ray, light_list);
        col = ray.col;

//#define REFLECTION
#ifdef REFLECTION
        Ray3D reflectedRay;
        getReflectedRay(ray, reflectedRay);

        Color reflectedColor = shadeRay(reflectedRay, scene, light_list, depth - 1);
        col = col + (ray.intersection.mat->specular * reflectedColor); // no += operator for color
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

	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			// Sets up ray origin and direction in view space, 
			// image plane is at z = -1.
			Point3D origin(0, 0, 0);
			Point3D imagePlane;
			imagePlane[0] = (-double(image.width)/2 + 0.5 + j)/factor;
			imagePlane[1] = (-double(image.height)/2 + 0.5 + i)/factor;
			imagePlane[2] = -1;

			
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
			Color col = shadeRay(ray, scene, light_list, depth);
			image.setColorAtPixel(i, j, col);			
		}
	}
}

