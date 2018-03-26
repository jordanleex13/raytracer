/***********************************************************
	
	Starter code for Assignment 3
	
	This file contains the interface and datastructures of the raytracer.  
	Simple traversal and addition code to the datastructures are given to you.

***********************************************************/
#pragma once

#include "util.h"
#include "scene_object.h"
#include "light_source.h"

class Raytracer {
public:
	// Renders 3D scene to an image given camera and lights setup.
	void render(Camera& camera, Scene& scene, LightList& light_list, Image& image);

	Raytracer() {
		srand(418);	// seed to make randomness predictable
	}

private:

	// Return the color of the ray after intersection and shading, call 
	// this function recursively for reflection and refraction.  
	Color shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int depth);

	// Traversal code for the scene, the ray is transformed into 
	// the object space of each node where intersection is performed.
	void traverseScene(Scene& scene, Ray3D& ray);

	// After intersection, calculate the color of the ray by shading it
	// with all light sources in the scene.
	void computeShading(Ray3D& ray, Scene& scene, LightList& light_list);

	// Precompute the modelToWorld and worldToModel transformations for each
    // object in the scene.
	void computeTransforms(Scene& scene);

	Ray3D getReflectedRay(Ray3D& ray);
	void getReflectedRay(Ray3D& ray, Ray3D& reflectedRay);

	// compute refracted rays from the given ray
	bool getRefractedRay(Ray3D& ray, Ray3D& refractedRay, float& transmittance);
	double generateRandom(double low=0.0, double high=1.0) {
		double f = (double)rand() / RAND_MAX;
		return low + f * (high - low);
	}

};
