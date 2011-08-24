#ifndef SCENE_H
#define SCENE_H
#include <QVector>
#include "geometry/shape.h"
#include "geometry/box.h"
#include "morten3d/Camera.h"
#include "arrays.h"
#include "node.h"
#include "sphere.h"
#include "spline.h"

class Scene
{
private:
    QVector<Node*> nodes;
    float resolution;

public:
    Camera camera;
    Sphere * sphere;
    Sphere * cursorSphere;
    Sphere * splineSphere;
    Box * box;
    Node * cursor;
    Node * activeNode;
    Spline * drawSpline;
    Scene() {

        resolution = 0.05f;
        sphere = new Sphere(0.05f, 255.0f, 0.0f, 0.0f);
        cursorSphere = new Sphere(0.05f, 0.0f,0.0f,0.0f);
        splineSphere = new Sphere(0.05f, 255.0f,0.0f,0.0f);
        cursor = new Node();
        cursor->shape = cursorSphere;
        cursor->position = Vector3(0,0,0);
        add(cursor);
        box = new Box(10,10,10);
        camera.trackMode = Camera::SPHERE_TRACK;
        camera.trackCenter = Vector3(box->getWidth()/2,box->getHeight()/2,-box->getDepth()/2);
        camera.trackSize = box->getWidth()*2;
        Node * drawBox = new Node();
        drawBox->shape = box;
        activeNode = drawBox;
        add(drawBox);

    }

    void showCursor(Vector3& point, Vector3& dir);
    void addPoint();

    void add(Node* node) {
        nodes.append(node);
    }

    QVector<Node*>& getNodes(){
        return nodes;
    }

    ~Scene() {
        foreach(Node* node , nodes) {
            delete node;
        }
        delete cursor;
    }

};

#endif // SCENE_H
