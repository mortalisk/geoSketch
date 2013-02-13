
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
    boxNode->scene = this;
    camera.setTrackMode(Camera::SPHERE_TRACK, Vector3(0,0,0), Vector3(10,10,10) );
    activeNode = boxNode;
    editLayerNo = -1;
    setSeaLevel = false;
}

void Scene::showCursor(Vector3& from, Vector3& direction) {
    float ss, tt;
    QVector<Vector3> points = activeNode->intersectionPoints(from,direction,ss,tt);
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
    if (setSeaLevel) {
        boxNode->setSeaLevel(cursor->position.y());
    } else if(activeNode){
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
        boxNode->makeLayer();
    }
}

void Scene::editLayer() {
    SurfaceNode * sn = qobject_cast<SurfaceNode*>(activeNode);
    if (sn != NULL && editLayerNo == -1) {
        for (int i = 0; i< boxNode->children.size(); ++i) {
            if (boxNode->children[i] == sn) {
                editLayerNo = i;
                break;
            }
        }
        boxNode->frontNode->uvSpline = sn->front;
        boxNode->leftNode->uvSpline = sn->left;
        boxNode->rightNode->uvSpline = sn->right;
        boxNode->backNode->uvSpline = sn->back;
        sn->visible = false;
        activeNode = boxNode;
    } else if(editLayerNo != -1) {


        SurfaceNode * node = qobject_cast<SurfaceNode*>(boxNode->children[editLayerNo]);
        node->front = boxNode->frontNode->uvSpline;
        node->left = boxNode->leftNode->uvSpline;
        node->back = boxNode->backNode->uvSpline;
        node->right = boxNode->rightNode->uvSpline;
        node->invalidate();
        node->visible = true;
        editLayerNo = -1;

        foreach(SideNode* s, boxNode->surfaces) {
            s->spline.clear();
            s->spline.isSuggestion = true;
        }

        bool hitIt = false;
        QVector<BaseNode*> changes;
        for (int i = 0; i<boxNode->children.size();i++) {
            BaseNode* s =  boxNode->children[i];
            if (s == node) {
                hitIt = true;
            } else if (hitIt) {
                changes.append(s);
                boxNode->children.remove(i);
                i--;
            }

        }
        boxNode->currentBelowNode = node->below;
        boxNode->updateCurrentBelowNode();
        foreach(BaseNode * n,changes) {
            SurfaceNode * s =  qobject_cast<SurfaceNode*>(n);
            s->below = boxNode->currentBelowNode;
            s->invalidate();
            boxNode->children.append(s);
            boxNode->updateCurrentBelowNode();
        }

        boxNode->waterNode->below = boxNode->currentBelowNode;
        boxNode->waterNode->invalidate();

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
    boxNode->scene = this;
    activeNode = boxNode;
}

Scene::~Scene() {
    delete boxNode;
}

