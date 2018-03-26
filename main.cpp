/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include <iostream>
#include "raytracer.h"

void scene_basic(int width, int height);

void scene_walls(int width, int height);

void scene_refrac(int width, int height);

int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.   
	int width = 320;
	int height = 240;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}
	
//	scene_basic(width, height);
	// scene_walls(width, height);
	scene_refrac(width, height);
	std::cout << "Finished main" << std::endl;

	return 0;
}

void scene_refrac(int width, int height){
	Raytracer raytracer;
	LightList light_list;
	Scene scene;
    Point3D origin(0,0,0);
	// Define materials for shading.
	Material transparent(Color(0.1, 0.1, 0.1), Color(1.0,1.0,0.0),
		Color(0.5,0.5,0.5),
		10.0, 1.33);

    Color darkgrey(0.5,0.5,0.5);
	Material slate(darkgrey, darkgrey, Color(0.1,0.1,0.1), 1.0);

	// Defines a point light source.
	PointLight* pLight = new PointLight(Point3D(10,10,10), Color(0.9,0.9,0.9));
	light_list.push_back(pLight);

	
	// Add a unit square into the scene with material mat.
	SceneNode* sphere = new SceneNode(new UnitSphere(), &transparent);
	scene.push_back(sphere);
	SceneNode* floor = new SceneNode(new UnitSquare(), &slate);
	scene.push_back(floor);


	// Apply some transformations to the sphere and unit square.
	double factor1[3] = { 5.0, 5.0, 5.0 };
	sphere->translate(Vector3D(0, 0, 0));
	sphere->scale(Point3D(0, 0, 0), factor1);

	double factor2[3] = { 500.0, 500.0, 1.0 };
	floor->scale(Point3D(0, 0, 0), factor2);

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
    Point3D cameraPositions[4] = {Point3D(0, -10, 5), Point3D(0, 10, 7), Point3D(10, 0, 10), Point3D(-10, 0, 3)};
    for (int i = 0; i < 4; i++) {
        Point3D cameraPos = cameraPositions[i];
        Camera camera1(cameraPos, origin - cameraPos, Vector3D(0, 0, 1), 60.0);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("view" + std::to_string(i) + ".bmp"); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }

	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

}

void scene_walls(int width, int height) {
	Raytracer raytracer;
	LightList light_list;
	Scene scene;
    Point3D origin(0,0,0);


    Material diffuseR( Color(0.2,0.0,0.0), Color(0.9,0.0,0.0), Color(0.1,0.1,0.1), 1.0);
    Material diffuseG( Color(0.0,0.2,0.0), Color(0.0,0.9,0.0), Color(0.1,0.1,0.1), 1.0);
    Material specularB( Color(0.0,0.0,0.2), Color(0.0,0.0,0.1), Color(1.0,1.0,1.0), 500.0);
    Material diffuseY( Color(0.2,0.2,0.0), Color(0.9,0.9,0.0), Color(0.0,0.0,0.0), 1.0);

    Material mirror(Color(0.001, 0.001, 0.001), Color(0.0, 0.0, 0.0), Color(0.999, 0.999, 0.999), 10000.0);

    Color darkgrey(0.5,0.5,0.5);
	Material slate(darkgrey, darkgrey, Color(0.1,0.1,0.1), 1.0);

	// Defines a point light source.
	PointLight* pLight = new PointLight(Point3D(10,10,10), Color(0.9,0.9,0.9));
	light_list.push_back(pLight);

	SceneNode* floor = new SceneNode(new UnitSquare(), &slate);
	scene.push_back(floor);
    SceneNode* globeG = new SceneNode(new UnitSphere(), &diffuseG);
    scene.push_back(globeG);
    SceneNode* globeR = new SceneNode(new UnitSphere(), &diffuseR);
    scene.push_back(globeR);
    SceneNode* globeB = new SceneNode(new UnitSphere(), &mirror);
    scene.push_back(globeB);
    SceneNode* globeY = new SceneNode(new UnitSphere(), &diffuseY);
    scene.push_back(globeY);

	SceneNode* wall = new SceneNode(new UnitSphere(), &specularB);
	scene.push_back(wall);

	// Apply some transformations to the sphere and unit square.
	double factor1[3] = { 500.0, 500.0, 1.0 };
	floor->scale(origin, factor1);

    double factor2[3] = {3.0,3.0,3.0};
    globeG->scale(origin, factor2);
    globeG->translate(Vector3D(-2, 0, 0));
    globeR->scale(origin, factor2);
    globeR->translate(Vector3D(2, 0, 0));
    globeB->scale(origin, factor2);
    globeB->translate(Vector3D(0, 1, 0));
    globeY->scale(origin, factor2);
    globeY->translate(Vector3D(0, -1.5, 0));

	wall->scale(origin, factor2);
	wall->translate(Vector3D(0,4,0));
	// Render the scene, feel free to make the image smaller for
	// testing purposes.

    Point3D cameraPositions[4] = {Point3D(0, -10, 5), Point3D(0, 10, 7), Point3D(10, 0, 10), Point3D(-10, 0, 3)};
    for (int i = 0; i < 4; i++) {
        Point3D cameraPos = cameraPositions[i];
        Camera camera1(cameraPos, origin - cameraPos, Vector3D(0, 0, 1), 60.0);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("view" + std::to_string(i) + ".bmp"); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }

	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}
}

void scene_basic(int width, int height){
	Raytracer raytracer;
	LightList light_list;
	Scene scene;
	// Define materials for shading.
	Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
		Color(0.628281, 0.555802, 0.366065),
		51.2);
	Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
		Color(0.316228,0.316228,0.316228),
		12.8);
	Material test(Color(0, 0, 0), Color(1.0,1.0,1.0),
		Color(1.0,1.0,1.0),
		1000.0);

	// Defines a point light source.
	PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
	light_list.push_back(pLight);
	
	// Add a unit square into the scene with material mat.
	SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
	scene.push_back(sphere);
	SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
	scene.push_back(plane);
//	SceneNode* cylinder = new SceneNode(new UnitCylinder(), &gold);
//	scene.push_back(cylinder);

	// Apply some transformations to the sphere and unit square.
	double factor1[3] = { 1.0, 2.0, 1.0 };
	sphere->translate(Vector3D(0, 0, -5));
	sphere->rotate('x', -45);
	sphere->rotate('z', 45);
	sphere->scale(Point3D(0, 0, 0), factor1);

	double factor2[3] = { 12.0, 12.0, 12.0 };
	plane->translate(Vector3D(0, 0, -15));
	plane->rotate('z', 45);
	plane->scale(Point3D(0, 0, 0), factor2);

//	double factor3[3] = { 1.0, 1.0, 1.0 };
//	cylinder->translate(Vector3D(2, 2, -10));
//	cylinder->rotate('x', 90);
//	cylinder->scale(Point3D(0, 0, 0), factor3);

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
	Image image1(width, height);
	raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
	image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

	// Render it from a different point of view.
	Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
	Image image2(width, height);
	raytracer.render(camera2, scene, light_list, image2);
	image2.flushPixelBuffer("view2.bmp");

	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

}