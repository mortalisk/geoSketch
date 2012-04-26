#ifndef SCENE_H
#define SCENE_H
#include <QVector>
#include "geometry/shape.h"
#include "morten3d/Camera.h"
#include "arrays.h"
#include "generalnode.h"
#include "sphere.h"
#include "spline.h"
#include "boxnode.h"

class Scene
{
private:
    float resolution;
    bool snapToGrid;
    bool onSurface;
    int editLayerNo;

public:

    Camera camera;
    Sphere sphere;
    Sphere cursorSphere;
    BoxNode * boxNode;
    BaseNode * cursor;
    BaseNode * activeNode;

    Scene();

    void showCursor(Vector3& point, Vector3& dir);
    void addPoint(Vector3& from, Vector3& direction);
    void selectActiveNode(Vector3& from, Vector3& direction);
    void makeLayer();
    void editLayer();

    BaseNode* getRootNode();

    Scene(Scene& scene)
    {
        *this=scene;
        boxNode = boxNode->copy();
        cursor = new GeneralNode(&cursorSphere, "cursor");
        cursor->ambient = scene.cursor->ambient;
        activeNode = boxNode;
    }

    ~Scene();

};

#endif // SCENE_H
