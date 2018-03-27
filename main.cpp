/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include <iostream>
#include "raytracer.h"

void scene_basic(int width, int height);
void scene_walls(int width, int height);
void scene_spheres(int, int);
void scene_refrac(int width, int height);

static Point3D origin(0,0,0);

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
    std::cout << "Width: " << width << " height: " << height << std::endl;
	
//	scene_basic(width, height);
	scene_spheres(width, height);
//    scene_walls(width, height);
//	scene_refrac(width, height);
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

        image1.flushPixelBuffer("refrac_view" + std::to_string(i) + ".bmp"); //save rendered image to file
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
    Point3D eye(0, 2, 10);
    Vector3D view(0, 0, -1);
    Vector3D up(0, 1, 0);
    double fov = 60;
    double aperture = 0.6;
    double focalLength = 12;

    Raytracer raytracer;
    LightList light_list;
    Scene scene;


    Material gold(Color(0.3, 0.3, 0.3),
                  Color(0.75164, 0.60648, 0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);

    Material jade(Color(0, 0, 0),
                  Color(0.54, 0.89, 0.63),
                  Color(0.316228, 0.316228, 0.316228),
                  12.8);

    Material red(Color(0.1, 0.0, 0.0),
                 Color(0.4, 0.4, 0.4),
                 Color(0.6, 0.05, 0.05),
                 1);

    Material blue(Color(0.0, 0.0, 0.1),
                  Color(0.4, 0.4, 0.4),
                  Color(0.05, 0.05, 0.6),
                  1);

    Material white(Color(0.01, 0.01, 0.01),
                   Color(0.5, 0.5, 0.5),
                   Color(0.5, 0.5, 0.5),
                   1);

    Material earth(Color(0.1, 0.1, 0.1),
                   Color(0.8, 0.8, 0.8),
                   Color(0.1, 0.1, 0.1),
                   10);

    Material silver(Color(0.19125, 0.19125, 0.19125),
                    Color(0.50754, 0.50754, 0.50754),
                    Color(0.508273, 0.508273, 0.508273),
                    100);

    Material glass(Color(0.001, 0.001, 0.001),
                   Color(0.0, 0.0, 0.0),
                   Color(0.999, 0.999, 0.999),
                   10000);

    Material mirror(Color(0.001, 0.001, 0.001),
                    Color(0.0, 0.0, 0.0),
                    Color(0.999, 0.999, 0.999),
                    10000);

    Material glossyMirror(Color(0.01, 0.01, 0.01),
                          Color(0.1, 0.1, 0.1),
                          Color(0.9, 0.9, 0.9),
                          1000);

    Material board(Color(0.01, 0.01, 0.01),
                   Color(0.09, 0.09, 0.09),
                   Color(0.9, 0.9, 0.9),
                   10000);

    // Defines a point light source.
    PointLight* pLight = new PointLight(Point3D(0,6,3), Color(0.2, 0.2, 0.2), Color(0.8, 0.8, 0.8), Color(0.8, 0.8, 0.8));
    light_list.push_back(pLight);

    // Construct scene
    SceneNode* floor = new SceneNode(new UnitSquare(), &glossyMirror);
    SceneNode* ceiling = new SceneNode(new UnitSquare(), &white);
    SceneNode* leftWall = new SceneNode(new UnitSquare(), &blue);
    SceneNode* rightWall = new SceneNode(new UnitSquare(), &red);
    SceneNode* backWall = new SceneNode(new UnitSquare(), &white);
    SceneNode* cylinder = new SceneNode(new UnitCylinder(), &gold);
    SceneNode* earthSphere = new SceneNode(new UnitSphere(), &earth);
    SceneNode* mirrorSphere = new SceneNode(new UnitSphere(), &mirror);
    SceneNode* glassSphere = new SceneNode(new UnitSphere(), &glass);

    scene.push_back(floor);
    scene.push_back(ceiling);
    scene.push_back(leftWall);
    scene.push_back(rightWall);
    scene.push_back(backWall);
//    scene.push_back(cylinder);
    scene.push_back(earthSphere);
    scene.push_back(mirrorSphere);
    scene.push_back(glassSphere);


    // Apply transformations
    double wallScale[3] = { 100.0, 100.0, 100.0 };
    floor->translate(Vector3D(0, -3, 0));
    floor->rotate('x', -90);
    floor->scale(origin, wallScale);

    backWall->translate(Vector3D(0, 0, -7));
    backWall->scale(origin, wallScale);

    leftWall->translate(Vector3D(-7, 0, 0));
    leftWall->rotate('y', 90);
    leftWall->scale(origin, wallScale);

    rightWall->translate(Vector3D(7, 0, 0));
    rightWall->rotate('y', -90);
    rightWall->scale(origin, wallScale);

    ceiling->translate(Vector3D(0, 7, 0));
    ceiling->rotate('x', 90);
    ceiling->scale(origin, wallScale);

//    double cylinderScale[3] = { 1.5, 2.0, 1.5 };
//    cylinder, Vector3D(-4, -2, -4));
//    cylinder, origin, cylinderScale);

    double sphereScale[3] = { 2.0, 2.0, 2.0 };
    earthSphere->translate(Vector3D(3, 3, -3));
    earthSphere->rotate('y', -25);
    earthSphere->rotate('z', -23.5);
    earthSphere->scale(origin, sphereScale);

    mirrorSphere->translate(Vector3D(-4, 0.9, -4));
    mirrorSphere->scale(origin, sphereScale);

    glassSphere->translate(Vector3D(1, -0.9, -0.5));

    Point3D cameraPos = eye;
    Camera camera1(cameraPos, view, up, fov);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

    image1.flushPixelBuffer("viewWalls.bmp"); //save rendered image to file
    std::cout << "Finished walls" << std::endl;
}

void scene_spheres(int width, int height) {
	Raytracer raytracer;
	LightList light_list;
	Scene scene;
    Point3D origin(0,0,0);


    Material diffuseR( Color(0.2,0.0,0.0), Color(0.9,0.0,0.0), Color(0.1,0.1,0.1), 1.0);
    Material diffuseG( Color(0.0,0.2,0.0), Color(0.0,0.9,0.0), Color(0.1,0.1,0.1), 1.0);
    Material specularB( Color(0.0,0.0,0.2), Color(0.0,0.0,0.1), Color(1.0,1.0,1.0), 500.0);
    Material diffuseY( Color(0.2,0.2,0.0), Color(0.9,0.9,0.0), Color(0.0,0.0,0.0), 1.0);

    Material mirror(Color(0.001, 0.001, 0.001), Color(0.0, 0.0, 0.0), Color(0.999, 0.999, 0.999), 10000.0);
	Material glossy(Color(0.01, 0.01, 0.01), Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9),1000.0);

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

        image1.flushPixelBuffer("sphere_view" + std::to_string(i) + ".bmp"); //save rendered image to file
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
	sphere->scale(origin, factor1);

	double factor2[3] = { 12.0, 12.0, 12.0 };
	plane->translate(Vector3D(0, 0, -15));
	plane->rotate('z', 45);
	plane->scale(origin, factor2);

//	double factor3[3] = { 1.0, 1.0, 1.0 };
//	cylinder->translate(Vector3D(2, 2, -10));
//	cylinder->rotate('x', 90);
//	cylinder->scale(origin, factor3);

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