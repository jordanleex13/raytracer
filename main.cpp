/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include <iostream>
#include "raytracer.h"

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))


void scene_basic(int width, int height);

void scene_cylinder(int width, int height);

void scene_walls(int width, int height);

void scene_spheres(int, int);

void scene_anti_aliasing(int width, int height);

void scene_refrac(int width, int height);

void scene_texture_map(int width, int height);

void scene_soft_shadows(int width, int height);

void scene_DOF(int width, int height);

void scene_wow(int width, int height);

static Point3D origin(0, 0, 0);

int main(int argc, char *argv[]) {
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

//    scene_basic(width, height);
//    scene_soft_shadows(width, height);
//    scene_texture_map(width, height);
//    scene_cylinder(width, height);
//    scene_anti_aliasing(width, height);
//    scene_spheres(width, height);
//    scene_walls(width, height);
//    scene_refrac(width, height);
//    scene_DOF(width, height);
//    scene_wow(width, height);

    std::cout << "Finished main" << std::endl;

    return 0;
}

void scene_anti_aliasing(int width, int height) {
    std::cout << "Rendering anti aliasing scene" << std::endl;
    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    Point3D origin(0, 0, 0);
    // Define materials for shading.
    Material transparent(Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(0.5, 0.5, 0.5), 1.0, 1.33, 0.9);
    Material diffuseR(Color(0.2, 0.0, 0.0), Color(0.9, 0.0, 0.0), Color(0.1, 0.1, 0.1), 1.0);
    Material blue(Color(0.0, 0.0, 0.1), Color(0.4, 0.4, 0.4), Color(0.05, 0.05, 0.6), 1);

    Color darkgrey(0.5, 0.5, 0.5);
    Material slate(darkgrey, darkgrey, Color(0.1, 0.1, 0.1), 1.0);

    // Defines a point light source.
    PointLight *pLight = new PointLight(Point3D(20, 20, 20), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);


    // Add a unit square into the scene with material mat.
    SceneNode *sphere = new SceneNode(new UnitSphere(), &blue);
    scene.push_back(sphere);
    SceneNode *sphere2 = new SceneNode(new UnitSphere(), &diffuseR);
    scene.push_back(sphere2);
    SceneNode *floor = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(floor);


    // Apply some transformations to the sphere and unit square.
    double factor1[3] = {2.0, 2.0, 2.0};
    sphere2->translate(Vector3D(-3, 0, 2));
    sphere2->scale(Point3D(0, 0, 0), factor1);

    sphere->translate(Vector3D(2, 0, 2));
    sphere->scale(Point3D(0, 0, 0), factor1);

    double factor2[3] = {500.0, 500.0, 1.0};
    floor->scale(Point3D(0, 0, 0), factor2);

    Point3D cameraPositions[4] = {Point3D(0, -10, 5), Point3D(0, 10, 7), Point3D(10, 0, 10), Point3D(-10, 0, 3)};
    for (int i = 0; i < 4; i++) {
        Point3D cameraPos = cameraPositions[i];
        Camera camera1(cameraPos, origin - cameraPos, Vector3D(0, 0, 1), 60.0);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("anti_aliasing" + std::to_string(i) + ".bmp"); //save rendered image to file
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

void scene_DOF(int width, int height) {
    std::cout << "Rendering depth of field scene" << std::endl;
    double fov = 60.0;
    double aperture = 0.6;
    double focalLength = 12.0;
    double focalRange = 4.0;

    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    Point3D origin(0, 0, 0);


    Material transparent(Color(0.1, 0.1, 0.1), Color(1.0, 1.0, 0.0), Color(0.5, 0.5, 0.5), 0.0, 1.33, 0.9);

    Material diffuseR(Color(0.2, 0.0, 0.0), Color(0.9, 0.0, 0.0), Color(0.1, 0.1, 0.1), 1.0);
    Material diffuseG(Color(0.0, 0.2, 0.0), Color(0.0, 0.9, 0.0), Color(0.1, 0.1, 0.1), 1.0);
    Material specularB(Color(0.0, 0.0, 0.2), Color(0.0, 0.0, 0.1), Color(1.0, 1.0, 1.0), 500.0);
    Material diffuseY(Color(0.2, 0.2, 0.0), Color(0.9, 0.9, 0.0), Color(0.0, 0.0, 0.0), 1.0);

    Material mirror(Color(0.001, 0.001, 0.001), Color(0.0, 0.0, 0.0), Color(0.999, 0.999, 0.999), 10000.0);
    Material glossy(Color(0.01, 0.01, 0.01), Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9), 1000.0);

    Color darkgrey(0.5, 0.5, 0.5);
    Material slate(darkgrey, darkgrey, Color(0.1, 0.1, 0.1), 1.0);

    // Defines a point light source.
    PointLight *pLight = new PointLight(Point3D(10, 10, 10), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

    SceneNode *floor = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(floor);
    SceneNode *globeG = new SceneNode(new UnitSphere(), &diffuseG);
    scene.push_back(globeG);
    SceneNode *globeR = new SceneNode(new UnitSphere(), &diffuseR);
    scene.push_back(globeR);
    SceneNode *globeB = new SceneNode(new UnitSphere(), &specularB);
    scene.push_back(globeB);
    SceneNode *globeY = new SceneNode(new UnitSphere(), &diffuseY);
    scene.push_back(globeY);

    SceneNode *globeM = new SceneNode(new UnitSphere(), &mirror);
    scene.push_back(globeM);

    // Apply some transformations to the sphere and unit square.
    double factor1[3] = {500.0, 500.0, 1.0};
    floor->scale(origin, factor1);

    double factor2[3] = {3.0, 3.0, 3.0};
    globeG->scale(origin, factor2);
    globeG->translate(Vector3D(-2, 10, 0));
    globeR->scale(origin, factor2);
    globeR->translate(Vector3D(0, 1, 0));
    globeB->scale(origin, factor2);
    globeB->translate(Vector3D(0, 5, 0));
    globeY->scale(origin, factor2);
    globeY->translate(Vector3D(0, 3, 0));

    globeM->scale(origin, factor2);
    globeM->translate(Vector3D(2, 10, 0));
    // Render the scene, feel free to make the image smaller for
    // testing purposes.

    Point3D cameraPositions[1] = {Point3D(0, -10, 5)};
    for (int i = 0; i < sizeof(cameraPositions) / sizeof(Point3D); i++) {
        Point3D cameraPos = cameraPositions[i];
        Camera camera1(cameraPos, origin - cameraPos, Vector3D(0, 0, 1), fov, aperture, focalLength, focalRange);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("DOF_view" + std::to_string(i) + ".bmp"); //save rendered image to file
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

void scene_refrac(int width, int height) {
    std::cout << "Rendering refraction scene" << std::endl;
    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    Point3D origin(0, 0, 0);
    // Define materials for shading.
    Material transparent(Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(0.5, 0.5, 0.5), 1.0, 1.33, 0.9);
    Material diffuseR(Color(0.2, 0.0, 0.0), Color(0.9, 0.0, 0.0), Color(0.1, 0.1, 0.1), 1.0);

    Color darkgrey(0.5, 0.5, 0.5);
    Material slate(darkgrey, darkgrey, Color(0.1, 0.1, 0.1), 1.0);

    // Defines a point light source.
    PointLight *pLight = new PointLight(Point3D(20, 20, 20), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);


    // Add a unit square into the scene with material mat.
    SceneNode *sphere = new SceneNode(new UnitSphere(), &transparent);
    scene.push_back(sphere);
    SceneNode *sphere2 = new SceneNode(new UnitSphere(), &diffuseR);
    scene.push_back(sphere2);
    SceneNode *floor = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(floor);
    SceneNode *left = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(left);
    SceneNode *right = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(right);
    SceneNode *top = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(top);
    SceneNode *back = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(back);


    // Apply some transformations to the sphere and unit square.
    double factor1[3] = {2.0, 2.0, 2.0};
    sphere2->translate(Vector3D(-3, 0, 2));
    sphere2->scale(Point3D(0, 0, 0), factor1);

    sphere->translate(Vector3D(2, 0, 2));
    sphere->scale(Point3D(0, 0, 0), factor1);

    double factor2[3] = {500.0, 500.0, 1.0};
    floor->scale(Point3D(0, 0, 0), factor2);

    left->scale(Point3D(0, 0, 0), factor2);
    left->rotate('x', 90);
    left->translate(Vector3D(0, -100, 0));


    right->scale(Point3D(0, 0, 0), factor2);
    right->rotate('x', 90);
    right->translate(Vector3D(0, 100, 0));


    top->scale(Point3D(0, 0, 0), factor2);
    top->rotate('x', 90);
    top->translate(Vector3D(0, 0, 100));


    back->scale(Point3D(0, 0, 0), factor2);
    back->rotate('x', 90);
    back->translate(Vector3D(-100, 0, 0));
    // double factor2[3] = { 500.0, 500.0, 1.0 };
    // floor->scale(Point3D(0, 0, 0), factor2);

    // left->scale(Point3D(0, 0, 0), factor2);
    // left->rotate('y', 90);
    // left->translate(Vector3D(10, 0, 0));


    // right->scale(Point3D(0, 0, 0), factor2);
    // right->rotate('y', 90);
    // right->translate(Vector3D(-10, 0, 0));


    // top->scale(Point3D(0, 0, 0), factor2);
    // top->rotate('x', 90);
    // top->translate(Vector3D(0, 10, 0));


    // back->scale(Point3D(0, 0, 0), factor2);
    // back->rotate('x', 90);
    // back->translate(Vector3D(0, -10, 0));


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

void scene_texture_map(int width, int height) {
    Raytracer raytracer;
    LightList light_list;
    Scene scene;

    Point3D north(Point3D(0, 6, 2));
    Point3D east(Point3D(6, 0, 2));
    Point3D south(Point3D(0, -6, 2));
    Point3D west(Point3D(-6, 0, 2));
    Point3D up(Point3D(0, 0, 6));
    Point3D down(Point3D(0, 0, -6));

    Color ambColor(1.0, 1.0, 1.0);
    Color difColor(1.0, 1.0, 1.0);
    Color speColor(0.0, 0.0, 0.0);
    // Defines a point light source.
    PointLight *pLight1 = new PointLight(north, ambColor, difColor, speColor);
    PointLight *pLight2 = new PointLight(east, ambColor, difColor, speColor);
    PointLight *pLight3 = new PointLight(south, ambColor, difColor, speColor);
    PointLight *pLight4 = new PointLight(west, ambColor, difColor, speColor);
    PointLight *pLight5 = new PointLight(up, ambColor, difColor, speColor);
    PointLight *pLight6 = new PointLight(down, ambColor, difColor, speColor);
    light_list.push_back(pLight1);
    light_list.push_back(pLight2);
    light_list.push_back(pLight3);
    light_list.push_back(pLight4);
    light_list.push_back(pLight5);
    light_list.push_back(pLight6);

    Texture textureEarth = Texture("texture_earth.bmp");
    Texture textureNumGrid = Texture("texture_numgrid.bmp");

    Material earth(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9), Color(0.1, 0.1, 0.1), 1);
    earth.texture = &textureEarth;
    Material numGrid(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9), Color(0.1, 0.1, 0.1), 1);
    numGrid.texture = &textureNumGrid;

    SceneNode *sphere = new SceneNode(new UnitSphere(), &earth);
    scene.push_back(sphere);
    double sphereScale[3] = {2.0, 2.0, 2.0};
    sphere->scale(origin, sphereScale);

    SceneNode *plane = new SceneNode(new UnitSquare(), &numGrid);
    scene.push_back(plane);
    double planeScale[3] = {10.0, 10.0, 1.0};
    plane->scale(origin, planeScale);
    plane->translate(Vector3D(0, 0, -4));   // move it down so it doesn't intersect sphere


    Point3D cameraPositions[6] = {north, east, south, west, Point3D(5, 5, 5), Point3D(-5, -5, -5)};
    for (int i = 0; i < NELEMS(cameraPositions); i++) {
        Point3D cameraPos = cameraPositions[i];
        Camera camera1(cameraPos, origin - cameraPos, Vector3D(0, 0, 1), 60.0);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
        image1.flushPixelBuffer("view_tm_" + std::to_string(i) + ".bmp"); //save rendered image to file
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
    std::cout << "Rendering walls scene" << std::endl;
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

    Texture textureEarth = Texture("texture_earth.bmp");
    Material earth(Color(0.1, 0.1, 0.1),
                   Color(0.8, 0.8, 0.8),
                   Color(0.1, 0.1, 0.1),
                   10);
    earth.texture = &textureEarth;

    Material silver(Color(0.19125, 0.19125, 0.19125),
                    Color(0.50754, 0.50754, 0.50754),
                    Color(0.508273, 0.508273, 0.508273),
                    100);

    Material glass(Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(0.5, 0.5, 0.5), 1.0, 1.33, 0.9);
    // Material glass(Color(0.001, 0.001, 0.001),
    //                Color(0.0, 0.0, 0.0),
    //                Color(0.999, 0.999, 0.999),
    //                10000);

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
    PointLight *pLight = new PointLight(Point3D(0, 6, 3), Color(0.2, 0.2, 0.2), Color(0.8, 0.8, 0.8),
                                        Color(0.8, 0.8, 0.8));
    light_list.push_back(pLight);

    // Construct scene
    SceneNode *floor = new SceneNode(new UnitSquare(), &glossyMirror);
    SceneNode *ceiling = new SceneNode(new UnitSquare(), &white);
    SceneNode *leftWall = new SceneNode(new UnitSquare(), &blue);
    SceneNode *rightWall = new SceneNode(new UnitSquare(), &red);
    SceneNode *backWall = new SceneNode(new UnitSquare(), &white);
    SceneNode *cylinder = new SceneNode(new UnitCylinder(), &gold);
    SceneNode *earthSphere = new SceneNode(new UnitSphere(), &earth);
    SceneNode *mirrorSphere = new SceneNode(new UnitSphere(), &mirror);
    SceneNode *glassSphere = new SceneNode(new UnitSphere(), &glass);

    scene.push_back(floor);
    scene.push_back(ceiling);
    scene.push_back(leftWall);
    scene.push_back(rightWall);
    scene.push_back(backWall);
    // scene.push_back(cylinder);
    scene.push_back(earthSphere);
    scene.push_back(mirrorSphere);
    scene.push_back(glassSphere);


    // Apply transformations
    double wallScale[3] = {100.0, 100.0, 100.0};
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

    // double cylinderScale[3] = { 1.5, 2.0, 1.5 };
    // cylinder, Vector3D(-4, -2, -4));
    // cylinder, origin, cylinderScale);

    double sphereScale[3] = {2.0, 2.0, 2.0};
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
    std::cout << "Rendering spheres scene" << std::endl;
    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    Point3D origin(0, 0, 0);


    Material transparent(Color(0.1, 0.1, 0.1), Color(1.0, 1.0, 0.0), Color(0.5, 0.5, 0.5), 0.0, 1.33, 0.9);

    Material diffuseR(Color(0.2, 0.0, 0.0), Color(0.9, 0.0, 0.0), Color(0.1, 0.1, 0.1), 1.0);
    Material diffuseG(Color(0.0, 0.2, 0.0), Color(0.0, 0.9, 0.0), Color(0.1, 0.1, 0.1), 1.0);
    Material specularB(Color(0.0, 0.0, 0.2), Color(0.0, 0.0, 0.1), Color(1.0, 1.0, 1.0), 500.0);
    Material diffuseY(Color(0.2, 0.2, 0.0), Color(0.9, 0.9, 0.0), Color(0.0, 0.0, 0.0), 1.0);

    Material mirror(Color(0.001, 0.001, 0.001), Color(0.0, 0.0, 0.0), Color(0.999, 0.999, 0.999), 10000.0);
    Material glossy(Color(0.01, 0.01, 0.01), Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9), 1000.0);

    Color darkgrey(0.5, 0.5, 0.5);
    Material slate(darkgrey, darkgrey, Color(0.1, 0.1, 0.1), 1.0);

    // Defines a point light source.
    PointLight *pLight = new PointLight(Point3D(10, 10, 10), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

    SceneNode *floor = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(floor);
    SceneNode *globeG = new SceneNode(new UnitSphere(), &diffuseG);
    scene.push_back(globeG);
    SceneNode *globeR = new SceneNode(new UnitSphere(), &diffuseR);
    scene.push_back(globeR);
    SceneNode *globeB = new SceneNode(new UnitSphere(), &mirror);
    scene.push_back(globeB);
    SceneNode *globeY = new SceneNode(new UnitSphere(), &transparent);
    scene.push_back(globeY);

    SceneNode *wall = new SceneNode(new UnitSphere(), &specularB);
    scene.push_back(wall);

    // Apply some transformations to the sphere and unit square.
    double factor1[3] = {500.0, 500.0, 1.0};
    floor->scale(origin, factor1);

    double factor2[3] = {3.0, 3.0, 3.0};
    globeG->scale(origin, factor2);
    globeG->translate(Vector3D(-2, 0, 0));
    globeR->scale(origin, factor2);
    globeR->translate(Vector3D(2, 0, 0));
    globeB->scale(origin, factor2);
    globeB->translate(Vector3D(0, 1, 0));
    globeY->scale(origin, factor2);
    globeY->translate(Vector3D(0, -1.5, 0));

    wall->scale(origin, factor2);
    wall->translate(Vector3D(0, 4, 0));
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


void scene_soft_shadows(int width, int height) {
    std::cout << "Rendering soft shadows" << std::endl;
    Raytracer raytracer;
    LightList light_list;
    Scene scene;

    Color darkgrey(0.5, 0.5, 0.5);
    Color smallSpec(0.1, 0.1, 0.1);
    Material diffuseR(Color(0.2, 0.0, 0.0), Color(0.9, 0.0, 0.0), smallSpec, 1.0);
    Material diffuseG(Color(0.0, 0.2, 0.0), Color(0.0, 0.9, 0.0), smallSpec, 1.0);
    Material slate(darkgrey, darkgrey, Color(0.1, 0.1, 0.1), 1.0);

    // Defines a point light source.
    PointLight *pLight = new PointLight(Point3D(0, 0, 20), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

    SceneNode *floor = new SceneNode(new UnitSquare(), &slate);
    SceneNode *redBall = new SceneNode(new UnitSphere(), &diffuseR);
    SceneNode *greenBall = new SceneNode(new UnitSphere(), &diffuseG);

    scene.push_back(floor);
    scene.push_back(redBall);
    scene.push_back(greenBall);

    double factor1[3] = {500.0, 500.0, 1.0};
    floor->scale(origin, factor1);

    double factor2[3] = {3.0, 3.0, 3.0};
    redBall->scale(origin, factor2);
    redBall->translate(Vector3D(-3, 0, 3));
    greenBall->scale(origin, factor2);
    greenBall->translate(Vector3D(3, 0, 3));

    Point3D cameraPositions[2] = {Point3D(20, 20, 20), Point3D(0, 40, 50)};
    for (int i = 0; i < NELEMS(cameraPositions); i++) {
        Point3D cameraPos = cameraPositions[i];
        Camera camera1(cameraPos, origin - cameraPos, Vector3D(0, 0, 1), 60.0);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("view_ss_" + std::to_string(i) + ".bmp"); //save rendered image to file
//        image1.flushPixelBuffer("view_no_ss_" + std::to_string(i) + ".bmp"); //save rendered image to file
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

void scene_cylinder(int width, int height) {

    std::cout << "Rendering cylinder scene" << std::endl;
    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54, 0.89, 0.63),
                  Color(0.316228, 0.316228, 0.316228),
                  12.8);

    // Defines a point light source.
    PointLight *pLight = new PointLight(Point3D(0, 0, 5), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

    // Add a unit square into the scene with material mat.
    SceneNode *cylinder = new SceneNode(new UnitCylinder(), &gold);
    scene.push_back(cylinder);
    SceneNode *plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);

    // Apply some transformations to the cylinder and unit square.
    double factor1[3] = {1.0, 1.0, 1.0};
    cylinder->translate(Vector3D(0, 0, -5));
    cylinder->rotate('x', 90);
    cylinder->scale(origin, factor1);

    double factor2[3] = {12.0, 12.0, 12.0};
    plane->translate(Vector3D(0, 0, -15));
    plane->rotate('z', 45);
    plane->scale(origin, factor2);

    // Render the scene, feel free to make the image smaller for testing purposes.
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


void scene_basic(int width, int height) {
    std::cout << "Rendering basic scene" << std::endl;
    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54, 0.89, 0.63),
                  Color(0.316228, 0.316228, 0.316228),
                  12.8);

    // Defines a point light source.
    PointLight *pLight = new PointLight(Point3D(0, 0, 5), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

    // Add a unit square into the scene with material mat.
    SceneNode *sphere = new SceneNode(new UnitCylinder(), &gold);
    scene.push_back(sphere);
    SceneNode *plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);

    // Apply some transformations to the sphere and unit square.
    double factor1[3] = {1.0, 2.0, 1.0};
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(origin, factor1);

    double factor2[3] = {12.0, 12.0, 12.0};
    plane->translate(Vector3D(0, 0, -15));
    plane->rotate('z', 45);
    plane->scale(origin, factor2);

    // Render the scene, feel free to make the image smaller for testing purposes.
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


void scene_wow(int width, int height) {
    std::cout << "Rendering wow scene" << std::endl;
    Raytracer raytracer;
    LightList light_list;
    Scene scene;

    // Defines a point light source.
    PointLight *pLight1 = new PointLight(Point3D(25, -10, 10), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight1);

    Color dim(0.1, 0.1, 0.1);
    Color bright(1.0, 1.0, 1.0);
    Material diffuseR(Color(0.2, 0.0, 0.0), Color(0.9, 0.0, 0.0), dim, 1.0);
    Material diffuseG(Color(0.0, 0.2, 0.0), Color(0.0, 0.9, 0.0), dim, 1.0);
    Material diffuseB(Color(0.0, 0.0, 0.2), Color(0.0, 0.0, 0.9), dim, 1.0);
    Material diffuseY(Color(0.2, 0.2, 0.0), Color(0.9, 0.9, 0.0), dim, 1.0);

    Material white(dim, bright, dim, 1.0);

    /* Walls */
    SceneNode *floor = new SceneNode(new UnitSquare(), &white);
    SceneNode *ceiling = new SceneNode(new UnitSquare(), &diffuseY);
    SceneNode *backWall = new SceneNode(new UnitSquare(), &diffuseR);
    SceneNode *leftWall = new SceneNode(new UnitSquare(), &diffuseG);
    SceneNode *rightWall = new SceneNode(new UnitSquare(), &diffuseB);
    scene.push_back(floor);
    scene.push_back(backWall);
    scene.push_back(leftWall);
    scene.push_back(rightWall);
    scene.push_back(ceiling);

    double wallScale[3] = {100.0, 100.0, 100.0};
    floor->translate(Vector3D(30, 0, 0));     // floor @ z = 0
    floor->scale(origin, wallScale);

    ceiling->translate(Vector3D(0, 0, 20));   // ceiling @ z = 20
    ceiling->scale(origin, wallScale);

    backWall->translate(Vector3D(0, 0, 20));
    backWall->rotate('y', 90);
    backWall->scale(origin, wallScale);

    leftWall->translate(Vector3D(0, -15, 0));
    leftWall->rotate('x', -90);
    leftWall->scale(origin, wallScale);

    rightWall->translate(Vector3D(0, 15, 0));
    rightWall->rotate('x', 90);
    rightWall->scale(origin, wallScale);


    /* Actual objects */
    Texture textureEarth = Texture("texture_earth.bmp");

    Material earth(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9), Color(0.1, 0.1, 0.1), 1.0);
    earth.texture = &textureEarth;
    Material mirror(Color(0.001, 0.001, 0.001), Color(0.0, 0.0, 0.0), Color(0.999, 0.999, 0.999), 10000.0);
    Material transparent(Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(0.5, 0.5, 0.5), 1.0, 1.33, 0.9);
    Material glossy(Color(0.01, 0.01, 0.01), Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9), 1000);


    SceneNode *globe = new SceneNode(new UnitSphere(), &earth);
    SceneNode *mirrorSphere1 = new SceneNode(new UnitSphere(), &mirror);
    SceneNode *mirrorSphere2 = new SceneNode(new UnitSphere(), &mirror);
    SceneNode *glassSphere = new SceneNode(new UnitSphere(), &transparent);

    scene.push_back(globe);
    scene.push_back(mirrorSphere1);
    scene.push_back(mirrorSphere2);
    scene.push_back(glassSphere);

    // transform objects
    double sphereScale[3] = {3.0, 3.0, 3.0};

    globe->scale(origin, sphereScale);
    globe->rotate('z', 180);
    globe->translate(Vector3D(-2.5, -3, 1.5));  // had to make all this negative to account for rotation

    mirrorSphere1->scale(origin, sphereScale);
    mirrorSphere1->translate(Vector3D(4, -1.75, 1));

    mirrorSphere2->scale(origin, sphereScale);
    mirrorSphere2->translate(Vector3D(1, -1, 3.5));

    double smallScale[3] = {1.5, 1.5, 1.5};
    glassSphere->scale(origin, smallScale);
    glassSphere->translate(Vector3D(12.0, 3.0, 2));

    Point3D cameraPositions[3] = {Point3D(35, 0, 10), Point3D(20, 4, 10), Point3D(55, 4, 10)};
    for (int i = 0; i < NELEMS(cameraPositions); i++) {
        Point3D cameraPos = cameraPositions[i];
        // look slightly above floor
        Camera camera1(cameraPos, Point3D(0, 0, 8) - cameraPos, Vector3D(0, 0, 1), 60.0);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("view_wow_" + std::to_string(i) + ".bmp"); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }

    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) { delete scene[i]; }
    for (size_t i = 0; i < light_list.size(); ++i) { delete light_list[i]; }
}