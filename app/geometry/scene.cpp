#include "scene.h"
#include <QVector>
#include <iostream>
#include "geometry/sphere.h"
#include "math.h"
#include "surface.h"

Scene::Scene() {
    snapToGrid = false;
    resolution = 0.05f;
    sphere = Sphere(0.05f);
    cursorSphere = Sphere(0.05f);
    cursor = new Node();
    cursor->shape = &cursorSphere;
    cursor->position = Vector3(0,0,0);
    boxNode = new BoxNode();
    boxNode->addChild(cursor);
    camera.setTrackMode(Camera::SPHERE_TRACK, Vector3(0,0,0), Vector3(10,10,10) );
    activeNode = boxNode;

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
}

void Scene::addPoint(Vector3& from, Vector3& direction) {
    if (onSurface) {
        activeNode->addPoint(from, direction);
    }
}

void Scene::makeLayer() {
    activeNode->makeLayer();
}

Node* Scene::getRootNode(){
    return boxNode;
}

Scene::~Scene() {
    delete cursor;
}

