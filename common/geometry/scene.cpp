#include "scene.h"
#include <QVector>
#include <iostream>
#include "geometry/sphere.h"

void Scene::addPoint(Vector3& from, Vector3& direction) {

    QVector<Vector3> points = activeNode->intersectionPoints(from,direction);
    if ( points.size() > 0) {
        Node* point = new Node();
        point->shape = sphere;
        point->position = points[0];
        add(point);
        std::cout << "adding new sphere" << std::endl;
    }

}
