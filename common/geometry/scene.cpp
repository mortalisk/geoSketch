#include "scene.h"
#include <QVector>
#include <iostream>
#include "geometry/sphere.h"
#include "math.h"

void Scene::showCursor(Vector3& from, Vector3& direction) {
    QVector<Vector3> points = activeNode->intersectionPoints(from,direction);
    if ( points.size() > 0) {
        float x = points[0].x();
        float y = points[0].y();
        float z = points[0].z();
        if (snapToGrid) {
            x = round( x / resolution)*resolution;
            y = round( y / resolution)*resolution;
            z = round( z / resolution)*resolution;
        }
        cursor->position = Vector3(x,y,z);
    }
}
void Scene::addPoint() {
    if (cursor != NULL) {
        int activeSpline = activeNode->splines.size();
        if (activeSpline < 1) {
            activeNode->addSpline();
        } else {
            activeSpline--;
        }
        Spline* spline = activeNode->splines[activeSpline];

        spline->addPoint(cursor->position);
        std::cout << "adding new point" << std::endl;
    }
}
