//
// Created by Jordan Lee on 2018-03-24.
//

#include "spatial.h"
#include "util.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

#include <boost/geometry/index/rtree.hpp>

#include <iostream>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<double, 3, bg::cs::cartesian> point_t;
typedef bg::model::box<point_t> box_t;
typedef std::pair<box_t, unsigned> value_t;

// To be used for intersections
bgi::rtree< value_t, bgi::quadratic<16> > m_rtree;


int addSceneObject(SceneNode* sceneNode, unsigned index) {

    // create a box
    Point3D bottomLeft = sceneNode->bvol.bottomLeft;
    Point3D topRight = sceneNode->bvol.topRight;

    point_t p1(bottomLeft[0], bottomLeft[1], bottomLeft[2]);
    point_t p2(topRight[0], topRight[1], topRight[2]);

    box_t b(p1, p2);
    m_rtree.insert(std::make_pair(b, index));

    return 0;
}