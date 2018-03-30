#ifndef OBJECTS
#define OBJECTS

#include <cstdlib>
#include <iostream>
#include "raytracer.h"

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

// Material glass(Color(1.0,1.0,1.0), Color(1.0,1.0,1.0), Color(1.0,1.0,1.0), 10.0, 1.33, 0.9);
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


// SceneNode* floor = new SceneNode(new UnitSquare(), &glossyMirror);
// SceneNode* ceiling = new SceneNode(new UnitSquare(), &white);
// SceneNode* leftWall = new SceneNode(new UnitSquare(), &blue);
// SceneNode* rightWall = new SceneNode(new UnitSquare(), &red);
// SceneNode* backWall = new SceneNode(new UnitSquare(), &white);
// SceneNode* cylinder = new SceneNode(new UnitCylinder(), &gold);
// SceneNode* earthSphere = new SceneNode(new UnitSphere(), &earth);
// SceneNode* mirrorSphere = new SceneNode(new UnitSphere(), &mirror);
// SceneNode* glassSphere = new SceneNode(new UnitSphere(), &glass);

#endif