#include "scene.h"
#include <QVector>
#include <iostream>
#include "geometry/sphere.h"
#include "math.h"

void Scene::showCursor(Vector3& from, Vector3& direction) {
    QVector<Vector3> points = activeNode->intersectionPoints(from,direction);
    if ( points.size() > 0) {
        float x = round(points[0].x() / resolution)*resolution;
        float y = round(points[0].y() / resolution)*resolution;
        float z = round(points[0].z() / resolution)*resolution;
        cursor->position = Vector3(x,y,z);
    }
}
void Scene::addPoint() {
    if (cursor != NULL) {
        Node* point = new Node();
        point->shape = sphere;
        point->position = cursor->position;
        add(point);
        std::cout << "adding new point" << std::endl;
    }
}
