/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include <iostream>
#include "raytracer.h"

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

// This is the same for part 1
void scene_basic(Raytracer&, LightList&, Scene&, int, int);

// These scenes showcase features
void scene_cylinder(Raytracer&, LightList&, Scene&, int, int);
void scene_spheres(Raytracer&, LightList&, Scene&, int, int);
void scene_glossy(Raytracer&, LightList&, Scene&, int, int);
void scene_anti_aliasing(Raytracer&, LightList&, Scene&, int, int);
void scene_refrac(Raytracer&, LightList&, Scene&, int, int);
void scene_texture_map(Raytracer&, LightList&, Scene&, int, int);
void scene_soft_shadows(Raytracer&, LightList&, Scene&, int, int);
void scene_DOF(Raytracer&, LightList&, Scene&, int, int);
void scene_wow(Raytracer&, LightList&, Scene&, int, int);
void scene_infinite_mirror(Raytracer&, LightList&, Scene&, int, int);

Point3D origin(0, 0, 0);

Color darkgrey(0.5, 0.5, 0.5);

Material gold(Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648), Color(0.628281, 0.555802, 0.366065), 51.2);
Material jade(Color(0, 0, 0), Color(0.54, 0.89, 0.63), Color(0.316228, 0.316228, 0.316228), 12.8);

Material transparent(Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(0.5, 0.5, 0.5), 1.0, 1.33, 0.9);
Material diffuseR(Color(0.2, 0.0, 0.0), Color(0.9, 0.0, 0.0), Color(0.1, 0.1, 0.1), 1.0);
Material diffuseG(Color(0.0, 0.2, 0.0), Color(0.0, 0.9, 0.0), Color(0.1, 0.1, 0.1), 1.0);
Material specularB(Color(0.0, 0.0, 0.2), Color(0.0, 0.0, 0.1), Color(1.0, 1.0, 1.0), 500.0);
Material diffuseY(Color(0.2, 0.2, 0.0), Color(0.9, 0.9, 0.0), Color(0.1, 0.1, 0.1), 1.0);
Material slate(darkgrey, darkgrey, Color(0.1, 0.1, 0.1), 1.0);

Material blue(Color(0.0, 0.0, 0.1), Color(0.4, 0.4, 0.4), Color(0.05, 0.05, 0.6), 1);

Material mirror(Color(0.001, 0.001, 0.001), Color(0.0, 0.0, 0.0), Color(0.999, 0.999, 0.999), 10000.0);
Material glossy(Color(0.01, 0.01, 0.01), Color(0.1, 0.1, 0.1), Color(0.8, 0.8, 0.8), 1000.0);



int main(int argc, char *argv[]) {

    int width = 320;
    int height = 240;

    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    std::cout << "Width: " << width << " Height: " << height << std::endl;

    Raytracer raytracer;
    LightList light_list;
    Scene scene;

    // Enable exactly 1 scene
     scene_basic(raytracer, light_list, scene, width, height);
    // scene_soft_shadows(raytracer, light_list, scene, width, height);
    // scene_texture_map(raytracer, light_list, scene, width, height);
    // scene_cylinder(raytracer, light_list, scene, width, height);
    // scene_anti_aliasing(raytracer, light_list, scene, width, height);
    // scene_spheres(raytracer, light_list, scene, width, height);
    // scene_glossy(raytracer, light_list, scene, width, height);
    // scene_refrac(raytracer, light_list, scene, width, height);
    // scene_DOF(raytracer, light_list, scene, width, height);
    // scene_wow(raytracer, light_list, scene, width, height);
//    scene_infinite_mirror(raytracer, light_list, scene, width, height);

    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
    std::cout << "Finished main" << std::endl;

    return 0;
}


void scene_infinite_mirror(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering infinite mirror scene" << std::endl;
    double fov = 60.0;
    double aperture = 0.6;
    double focalLength = 6.0;
    double focalRange = 4.0;

    PointLight *pLight1 = new PointLight(Point3D(25, -10, 10), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight1);

    Color dim(0.1, 0.1, 0.1);

    Material babyblue(Color(0.05, 0.15, 0.2), Color(0, 0.4, 0.6), dim, 1.0);
    Material mirror(Color(0.001, 0.001, 0.001), Color(0.0, 0.0, 0.0), Color(0.999, 0.999, 0.999), 10000.0);

    /* Walls */
    SceneNode *floor = new SceneNode(new UnitSquare(), &babyblue);
    SceneNode *ceiling = new SceneNode(new UnitSquare(), &mirror);
    SceneNode *backWall = new SceneNode(new UnitSquare(), &mirror);
    SceneNode *frontWall = new SceneNode(new UnitSquare(), &mirror);
    SceneNode *leftWall = new SceneNode(new UnitSquare(), &mirror);
    SceneNode *rightWall = new SceneNode(new UnitSquare(), &mirror);
    scene.push_back(floor);
    scene.push_back(ceiling);
    scene.push_back(backWall);
    scene.push_back(frontWall);
    scene.push_back(leftWall);
    scene.push_back(rightWall);

    double wallScale[3] = {300.0, 300.0, 300.0};
    floor->translate(Vector3D(30, 0, 0));     // floor @ z = 0
    floor->scale(origin, wallScale);

    ceiling->translate(Vector3D(0, 0, 20));   // ceiling @ z = 20
    ceiling->scale(origin, wallScale);

    backWall->translate(Vector3D(0, 0, 20));
    backWall->rotate('y', 90);
    backWall->scale(origin, wallScale);    

    frontWall->translate(Vector3D(55, 0, 20));
    frontWall->rotate('y', -90);
    frontWall->scale(origin, wallScale);

    leftWall->translate(Vector3D(0, -15, 0));
    leftWall->rotate('x', -90);
    leftWall->scale(origin, wallScale);

    rightWall->translate(Vector3D(0, 15, 0));
    rightWall->rotate('x', 90);
    rightWall->scale(origin, wallScale);


    /* Actual objects */
    Texture textureEarth = Texture("../resources/texture_earth.bmp");
    Texture textureGranite = Texture("../resources/texture_granite.bmp");
    Texture textureIce = Texture("../resources/texture_ice.bmp");
    Texture textureRainbow = Texture("../resources/texture_rainbow.bmp");

    Material earth(Color(0.2, 0.2, 0.2), Color(0.9, 0.9, 0.9), dim, 1.0);
    Material granite(Color(0.2, 0.2, 0.2), Color(0.9, 0.9, 0.9), dim, 10.0);
    Material ice(Color(0.2, 0.2, 0.2), Color(0.9, 0.9, 0.9), dim, 10.0);
    Material rainbow(Color(0.2, 0.2, 0.2), Color(0.9, 0.9, 0.9), dim, 10.0);
    earth.texture = &textureEarth;
    granite.texture = &textureGranite;
    ice.texture = &textureIce;
    rainbow.texture = &textureRainbow;

    Material transparent(Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(0.5, 0.5, 0.5), 1.0, 1.33, 0.95);

    SceneNode *globe = new SceneNode(new UnitSphere(), &earth);
    SceneNode *stone = new SceneNode(new UnitSphere(), &granite);
    SceneNode *iceball = new SceneNode(new UnitSphere(), &ice);
    SceneNode *colorfulball = new SceneNode(new UnitSphere(), &rainbow);
    SceneNode *mirrorSphere1 = new SceneNode(new UnitSphere(), &mirror);
    SceneNode *mirrorSphere2 = new SceneNode(new UnitSphere(), &mirror);
    SceneNode *glassSphere = new SceneNode(new UnitSphere(), &transparent);

    scene.push_back(globe);
    scene.push_back(stone);
    scene.push_back(iceball);
    scene.push_back(colorfulball);
    scene.push_back(mirrorSphere1);
    scene.push_back(mirrorSphere2);
    scene.push_back(glassSphere);

    // transform objects
    double sphereScale[3] = {3.0, 3.0, 3.0};

    globe->scale(origin, sphereScale);
    globe->rotate('z', 180);
    globe->translate(Vector3D(-2.5, 3, 3.0));  // had to make all this negative to account for rotation

    mirrorSphere1->scale(origin, sphereScale);
    mirrorSphere1->translate(Vector3D(4, -2.5, 1.5));

    mirrorSphere2->scale(origin, sphereScale);
    mirrorSphere2->translate(Vector3D(1, -1.5, 3.5));

    stone->scale(origin, sphereScale);
    stone->translate(Vector3D(4, 0.5, 2.0));

    iceball->scale(origin, sphereScale);
    iceball->translate(Vector3D(4, 2.5, 4.0));

    colorfulball->scale(origin, sphereScale);
    colorfulball->translate(Vector3D(4.5, 0, 4.5));

    double smallScale[3] = {1.5, 1.5, 1.5};
    double largeScale[3] = {4.0, 4.0, 4.0};
    glassSphere->scale(origin, largeScale);
    glassSphere->translate(Vector3D(4.0, 1.5, 2.0));

    Point3D cameraPositions[3] = {Point3D(20, -10, 10), Point3D(35, 0, 10), Point3D(55, 4, 20)};
    Point3D viewDir[3] = {Point3D(0,0,-4), Point3D(0,0,8), Point3D(0,0,8)};
    double focalLengths[3] = {2, 6, 12};
    double focalRanges[3] = {4, 4, 4};
    for (int i = 0; i < NELEMS(cameraPositions); i++) {
        Point3D cameraPos = cameraPositions[i];
        // look slightly above floor
        Camera camera1(cameraPos, viewDir[i] - cameraPos, Vector3D(0, 0, 1), fov, aperture, focalLengths[i], focalRanges[i]);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("view_infinite_mirrior" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }
}


void scene_glossy(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering glossy scene" << std::endl;

    PointLight *pLight = new PointLight(Point3D(20, 20, 20), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

    SceneNode *sphere = new SceneNode(new UnitSphere(), &glossy);
    scene.push_back(sphere);
    SceneNode *sphere2 = new SceneNode(new UnitSphere(), &blue);
    scene.push_back(sphere2);
    SceneNode *floor = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(floor);


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
        image1.flushPixelBuffer("glossy" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }

}


void scene_anti_aliasing(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering anti aliasing scene" << std::endl;

    PointLight *pLight = new PointLight(Point3D(20, 20, 20), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);


    SceneNode *sphere = new SceneNode(new UnitSphere(), &blue);
    scene.push_back(sphere);
    SceneNode *sphere2 = new SceneNode(new UnitSphere(), &diffuseR);
    scene.push_back(sphere2);
    SceneNode *floor = new SceneNode(new UnitSquare(), &slate);
    scene.push_back(floor);


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

        image1.flushPixelBuffer("anti_aliasing" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }

}

void scene_DOF(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering depth of field scene" << std::endl;
    double fov = 60.0;
    double aperture = 0.6;
    double focalLength = 12.0;
    double focalRange = 4.0;

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

    Point3D cameraPositions[1] = {Point3D(0, -10, 5)};
    for (int i = 0; i < sizeof(cameraPositions) / sizeof(Point3D); i++) {
        Point3D cameraPos = cameraPositions[i];
        Camera camera1(cameraPos, origin - cameraPos, Vector3D(0, 0, 1), fov, aperture, focalLength, focalRange);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("DOF_view" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }

}

void scene_refrac(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering refraction scene" << std::endl;

    PointLight *pLight = new PointLight(Point3D(20, 20, 20), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

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

        image1.flushPixelBuffer("refrac_view" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }

}

void scene_texture_map(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering texture map scene" << std::endl;
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

    Texture textureEarth = Texture("../resources/texture_earth.bmp");
    Texture textureNumGrid = Texture("../resources/texture_board.bmp");

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
        image1.flushPixelBuffer("view_tm_" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }
}

void scene_spheres(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering spheres scene" << std::endl;

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

    Point3D cameraPositions[4] = {Point3D(0, -10, 5), Point3D(0, 10, 7), Point3D(10, 0, 10), Point3D(-10, 0, 3)};
    for (int i = 0; i < 4; i++) {
        Point3D cameraPos = cameraPositions[i];
        Camera camera1(cameraPos, origin - cameraPos, Vector3D(0, 0, 1), 60.0);
        Image image1(width, height);
        raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image

        image1.flushPixelBuffer("sphere_view" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }
}


void scene_soft_shadows(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering soft shadows" << std::endl;

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

        image1.flushPixelBuffer("view_ss_" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
       image1.flushPixelBuffer("view_no_ss_" /*+ std::to_string(i)*/ + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }

}

void scene_cylinder(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {

    std::cout << "Rendering cylinder scene" << std::endl;

    PointLight *pLight = new PointLight(Point3D(0, 0, 5), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

    SceneNode *cylinder = new SceneNode(new UnitCylinder(), &gold);
    scene.push_back(cylinder);
    SceneNode *plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);

    double factor1[3] = {1.0, 1.0, 1.0};
    cylinder->translate(Vector3D(0, 0, -5));
    cylinder->rotate('x', 90);
    cylinder->scale(origin, factor1);

    double factor2[3] = {12.0, 12.0, 12.0};
    plane->translate(Vector3D(0, 0, -15));
    plane->rotate('z', 45);
    plane->scale(origin, factor2);

    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("view2.bmp");

}


void scene_basic(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering basic scene" << std::endl;

    // Defines a point light source.
    PointLight *pLight = new PointLight(Point3D(0, 0, 5), Color(0.9, 0.9, 0.9));
    light_list.push_back(pLight);

    // Add a unit square into the scene with material mat.
    SceneNode *sphere = new SceneNode(new UnitSphere(), &gold);
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

    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("view2.bmp");

}


void scene_wow(Raytracer& raytracer, LightList& light_list, Scene& scene, int width, int height) {
    std::cout << "Rendering wow scene" << std::endl;

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
    Texture textureEarth = Texture("../resources/texture_earth.bmp");

    Material earth(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9), Color(0.1, 0.1, 0.1), 1.0);
    earth.texture = &textureEarth;
    Material mirror(Color(0.001, 0.001, 0.001), Color(0.0, 0.0, 0.0), Color(0.999, 0.999, 0.999), 10000.0);
    Material transparent(Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(0.5, 0.5, 0.5), 1.0, 1.33, 0.9);

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

        image1.flushPixelBuffer("view_wow_" + std::to_string(i) + std::string(".bmp")); //save rendered image to file
        std::cout << "Finished " << i << std::endl;
    }
}