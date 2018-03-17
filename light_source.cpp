/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include <algorithm>
#include "light_source.h"

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.

    // Get all vectors needed for Phong shading
    Vector3D L = this->pos - ray.intersection.point;
    L.normalize();
    Vector3D N = ray.intersection.normal;
    N.normalize();
    Vector3D R = 2*(L.dot(N))*N - L;
    R.normalize();
    Vector3D V = -ray.dir;    // ray.dir is from eye to point. V is point to eye
    V.normalize();

    // Compute ambient, diffuse, specular components
    Material* mat = ray.intersection.mat;
    Color amb = mat->ambient * col_ambient;
    amb.clamp();
    Color diff = mat->diffuse * (std::max(0.0, N.dot(L)) * col_diffuse);
    diff.clamp();
    Color spec = mat->specular * (std::max(0.0, std::pow(R.dot(V), mat->specular_exp)) * col_specular);
    spec.clamp();

    // assign the colour to the ray
    ray.col = amb + diff + spec;
    ray.col.clamp();
}

