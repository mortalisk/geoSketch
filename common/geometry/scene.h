#ifndef SCENE_H
#define SCENE_H
#include <QVector>
#include "geometry/shape.h"
#include "geometry/box.h"
#include "morten3d/Camera.h"
#include "arrays.h"
#include "node.h"
#include "sphere.h"

class Scene
{
private:
    QVector<Node*> nodes;

public:
    Camera camera;
    Sphere * sphere;
    Box * box;
    Node * activeNode;
    Scene() {
        sphere = new Sphere(0.01f);
        box = new Box(1,1,1);
        Node * drawBox = new Node();
        drawBox->shape = box;
        activeNode = drawBox;
        add(drawBox);
    }

    void addPoint(Vector3& point, Vector3& dir);

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
    }

};

#endif // SCENE_H
