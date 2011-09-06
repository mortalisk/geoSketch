#include "scene.h"
#include <QVector>
#include <iostream>
#include "geometry/sphere.h"
#include "math.h"
#include "surface.h"

Scene::Scene() {
    snapToGrid = false;
    resolution = 0.05f;
    sphere = new Sphere(0.05f, 255.0f, 0.0f, 0.0f);
    cursorSphere = new Sphere(0.05f, 255.0f,0.0f,0.0f);
    splineSphere = new Sphere(0.05f, 255.0f,0.0f,0.0f);
    cursor = new Node();
    cursor->shape = cursorSphere;
    cursor->position = Vector3(0,0,0);
    add(cursor);
    boxNode = new BoxNode();
    camera.setTrackMode(Camera::SPHERE_TRACK, Vector3(boxNode->getWidth()/2,boxNode->getHeight()/2,-boxNode->getDepth()/2), Vector3(10,10,10) );
    activeNode = boxNode;
    add(boxNode);
    void draw();

}

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
        onSurface = true;
    } else {
        Vector3 pos = camera.position + direction.normalize()*5;
        cursor->position = Vector3(pos.x(),pos.y(),pos.z());
        onSurface = false;
    }

    std::cout << "cursor: " << cursor->position << std::endl <<
                 "from: " << from << std::endl <<
                 "direction: " << direction << std::endl;
}

void Scene::addPoint() {
    if (onSurface) {
        activeNode->addPoint(cursor->position);
    }
}

void Scene::makeLayer() {
    activeNode->makeLayer();
}

void Scene::add(Node* node) {
    nodes.append(node);
}

QVector<Node*>& Scene::getNodes(){
    return nodes;
}

Scene::~Scene() {
    foreach(Node* node , nodes) {
        delete node;
    }
    delete cursor;
}

