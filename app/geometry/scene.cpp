
#define _USE_MATH_DEFINES
#include "scene.h"
#include "util.h"
#include <QVector>
#include <QVariantMap>
#include <iostream>
#include "geometry/sphere.h"
#include <math.h>
#include "surface.h"
#include "surfacenode.h"
#include "json.h"

Scene::Scene() {
    snapToGrid = false;
    resolution = 0.05f;
    sphere = Sphere(0.05f);
    cursorSphere = Sphere(0.05f);
    cursor = new GeneralNode(&cursorSphere,"cursor");
    cursor->position = Vector3(0,0,0);
    cursor->ambient = QVector4D(1.0, 0.0, 0.0, 1.0);
    boxNode = new BoxNode();
    camera.setTrackMode(Camera::SPHERE_TRACK, Vector3(0,0,0), Vector3(10,10,10) );
    activeNode = boxNode;
    editLayerNo = -1;
    seaLevel = 0.0;
    seaActive = false;
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
    //if (onSurface) {
    if(activeNode){
        activeNode->addPoint(from, direction);
    }
}

void Scene::selectActiveNode(Vector3 &from, Vector3 &direction) {

    Vector3 point;
    activeNode->setActive(false);
    activeNode = boxNode->findIntersectingNode(from, direction, point);
    if (activeNode == NULL)
        activeNode = boxNode;
    activeNode->setActive(true);

}

void Scene::makeLayer() {
    if (editLayerNo == -1) {
        activeNode = boxNode->makeLayer();
    }
}

void Scene::editLayer() {
    SurfaceNode * sn = dynamic_cast<SurfaceNode*>(activeNode);
    if (sn != NULL && editLayerNo == -1) {
        for (int i = 0; i< boxNode->children.size(); ++i) {
            if (boxNode->children[i] == sn) {
                editLayerNo = i;
                break;
            }
        }
        boxNode->frontNode->spline = sn->front;
        boxNode->leftNode->spline = sn->left;
        boxNode->rightNode->spline = sn->right;
        boxNode->backNode->spline = sn->back;
        sn->visible = false;
        activeNode = boxNode;
    } else if(editLayerNo != -1) {


        SurfaceNode * node = dynamic_cast<SurfaceNode*>(boxNode->children[editLayerNo]);
        node->front = boxNode->frontNode->spline;
        node->left = boxNode->leftNode->spline;
        node->back = boxNode->backNode->spline;
        node->right = boxNode->rightNode->spline;
        node->invalidate();
        node->visible = true;
        editLayerNo = -1;

        foreach(SideNode* s, boxNode->surfaces) {
            s->spline.clear();
            s->spline.isSuggestion = true;
        }
        foreach(BaseNode* s, boxNode->children) {
            SurfaceNode * node2 = dynamic_cast<SurfaceNode*>(s);
            if (node2 && node2->below == node) {
                node2->invalidate();
            }
        }
    }
}

BaseNode* Scene::getRootNode(){
    return boxNode;
}

QVariantMap Scene::toJson() {
    QVariantMap map;
    map["resolution"] = resolution;
    map["boxNode"] = boxNode->toJson();
    return map;
}

void Scene::fromJson(QVariantMap &map) {
    resolution = map["resolution"].toDouble();
    delete boxNode;
    boxNode = new BoxNode();
    boxNode->fromJson(map["boxNode"].toMap());
    activeNode = boxNode;
}

Scene::~Scene() {
    delete boxNode;
}

