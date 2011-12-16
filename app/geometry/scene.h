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
#include "boxnode.h"

class Scene
{
private:
    float resolution;
    bool snapToGrid;
    bool onSurface;

public:

    Camera camera;
    Sphere sphere;
    Sphere cursorSphere;
    BoxNode * boxNode;
    Node * cursor;
    Node * activeNode;

    Scene();

    void showCursor(Vector3& point, Vector3& dir);
    void addPoint(Vector3& from, Vector3& direction);
    void makeLayer();

    Node* getRootNode();

    Scene(Scene& scene)
    {
        *this=scene;
        boxNode = boxNode->copy();
        cursor = new Node(&cursorSphere, "cursor");

        activeNode = boxNode;
    }

    ~Scene();

};

#endif // SCENE_H
