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
    QVector<Node*> nodes;
    float resolution;
    bool snapToGrid;
    bool onSurface;

public:
    Camera camera;
    Sphere * sphere;
    Sphere * cursorSphere;
    Sphere * splineSphere;
    BoxNode * boxNode;
    Node * cursor;
    Node * activeNode;
    Spline * drawSpline;
    Scene();

    void showCursor(Vector3& point, Vector3& dir);
    void addPoint();
    void makeLayer();

    void add(Node* node);

    QVector<Node*>& getNodes();

    ~Scene();

};

#endif // SCENE_H
