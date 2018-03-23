/***********************************************************
	
	Starter code for Assignment 3
	
	Implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include <cassert>
#include "scene_object.h"
#include <vector>

bool UnitCylinder::intersect(Ray3D& ray, const Matrix4x4& worldToModel, const Matrix4x4& modelToWorld) {
	// defining a unit cylinder with radius = 1, height = 1, centered at origin (0,0,0)

	// convert to object space
	Point3D eye = worldToModel * ray.origin;
	Vector3D eyeToPlane = worldToModel * ray.dir;

	// Center of unit sphere
	Point3D center(0.0, 0.0, 0.0);

	Vector3D centerToEye = eye - center;
	// intersecting with an infinite long cylinder
	double a = eyeToPlane[0]*eyeToPlane[0] + eyeToPlane[1]*eyeToPlane[1];
	double b = 2 * (centerToEye[0]*eyeToPlane[0] + centerToEye[1]*eyeToPlane[1]);
	double c = centerToEye[0]*centerToEye[0] + centerToEye[1]*centerToEye[1] - 1; // 1 is R^2

	std::vector<double> time;
	double determinant = b*b - 4*a*c;
	if (determinant >= 0){
		double t1 = (-b + std::sqrt(determinant)) / (2*a);
		double t2 = (-b - std::sqrt(determinant)) / (2*a);
		if (t1 >= 0) { time.push_back(t1); }
		if (t2 >= 0) { time.push_back(t2); }
	}

	// intersecting with top and bottom planes
	Vector3D N = Vector3D(0.0, 0.0, 1.0);
	Point3D p0 = Point3D(0.0, 0.0, 0.5);
	Point3D p1 = Point3D(0.0, 0.0, -0.5);
	if (eyeToPlane.dot(N) != 0) {
		// equation to determine time of intersection
		double t1 = (p0-eye).dot(N) / (eyeToPlane.dot(N));
		double t2 = (p1-eye).dot(N) / (eyeToPlane.dot(N));
		if (t1 >= 0) { time.push_back(t1); }
		if (t2 >= 0) { time.push_back(t2); }
	}

	if (time.empty()) { return false; }
	// check if all points lie on cylinder
	double t = -1;
	for(auto it = time.begin(); it != time.end(); ++it) {
		Point3D intersection = eye + (*it) * eyeToPlane;
		if (intersection[2] < -0.5 || intersection[2] > 0.5) { continue; }
	    if (intersection[0]*intersection[0] + intersection[1]*intersection[1] > 1) { continue; }
	    if (t == -1) { t = (*it); }
	    else { t = std::min(t, (*it)); }
	}
	if (t == -1) {return false;}
	// use line equation to get intersection point
	Point3D intersection = eye + t * eyeToPlane;
	Vector3D normal;
	if (intersection[3]==0.5){normal=transNorm(worldToModel, Vector3D(0,0,1));}
	if (intersection[3]==(-0.5)){normal=transNorm(worldToModel, Vector3D(0,0,-1));}
	else {normal=transNorm(worldToModel, Vector3D(intersection[0], intersection[1], 0));}

	// check if already a valid intersection, update if this one is closer
	if (ray.intersection.none || t < ray.intersection.t_value) {
		// std::cout << "( " << intersection[0] << ", " << intersection[1] << ", " << intersection[2] << ")" << std::endl;
		ray.intersection.point = modelToWorld * intersection;

		ray.intersection.normal = normal;
		ray.intersection.normal.normalize();

		ray.intersection.t_value = t;
		ray.intersection.none = false;
		return true;
	}
	return false;
}

bool UnitSquare::intersect(Ray3D &ray, const Matrix4x4 &worldToModel, const Matrix4x4 &modelToWorld) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	// convert to object space
	Point3D eye = worldToModel * ray.origin;
	Vector3D eyeToPlane = worldToModel * ray.dir;

	// get necessary vectors for calculation
	Vector3D N = Vector3D(0.0, 0.0, 1.0);
	Point3D p0 = Point3D(0.0, 0.0, 0.0);	// any point on plane would work
	if (eyeToPlane.dot(N) == 0) {
		std::cout << "No intersection or light is in plane" << std::endl;
		return false;
	}
	// equation to determine time of intersection
	double t = (p0-eye).dot(N) / (eyeToPlane.dot(N));
	if (t <= 0)
		return false;

    // use line equation to get intersection point
	Point3D planeIntersection = eye + t * eyeToPlane;
	double x = planeIntersection[0];
	double y = planeIntersection[1];

	// check if inside unit square
	if (x < 0.5 && x > -0.5 && y < 0.5 && y > -0.5) {
		// check if already a valid intersection, update if this one is closer
		if (ray.intersection.none || t < ray.intersection.t_value) {

			ray.intersection.point = modelToWorld * planeIntersection;	// convert back to world

			// TODO i assume i don't need to do the inverse transpose and that the function does it for me
			ray.intersection.normal = transNorm(worldToModel, N);
			ray.intersection.normal.normalize();

			ray.intersection.t_value = t;
			ray.intersection.none = false;	// there is an intersection
			return true;
		}
	}
	return false;
}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel, const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	// convert to object space
	Point3D eye = worldToModel * ray.origin;
	Vector3D eyeToPlane = worldToModel * ray.dir;

	// Center of unit sphere
	Point3D center(0.0, 0.0, 0.0);

	Vector3D centerToEye = eye - center;
	// solution to t is a quadratic equation after doing the math
	double a = eyeToPlane.dot(eyeToPlane);
	double b = 2 * eyeToPlane.dot(centerToEye);
	double c = centerToEye.dot(centerToEye) - 1; // 1 is R^2

	double determinant = b*b - 4*a*c;
	if (determinant < 0)
		return false;
	double t1 = (-b + std::sqrt(determinant)) / (2*a);
	double t2 = (-b - std::sqrt(determinant)) / (2*a);

	// select the smallest t -> first intersection
	double t = std::min(t1, t2);
	// assert(t > 0);
	if (t1 < 0 && t2 < 0) return false;
	if (t1 < 0) t = t2;
	if (t2 < 0) t = t1;

	// use line equation to get intersection point
	Point3D sphereIntersection = eye + t * eyeToPlane;

	// check if already a valid intersection, update if this one is closer
	if (ray.intersection.none || t < ray.intersection.t_value) {
		ray.intersection.point = modelToWorld * sphereIntersection;

		ray.intersection.normal = transNorm(worldToModel, sphereIntersection - center);
		ray.intersection.normal.normalize();

		ray.intersection.t_value = t;
		ray.intersection.none = false;
		return true;
	}
	return false;
}

void SceneNode::rotate(char axis, double angle) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;
	
	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			this->trans = this->trans*rotation; 	
			angle = -angle;
		} 
		else {
			this->invtrans = rotation*this->invtrans; 
		}	
	}
}

void SceneNode::translate(Vector3D trans) {
	Matrix4x4 translation;
	
	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	this->trans = this->trans*translation; 	
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	this->invtrans = translation*this->invtrans; 
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
	Matrix4x4 scale;
	
	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	this->trans = this->trans*scale; 	
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	this->invtrans = scale*this->invtrans; 
}


