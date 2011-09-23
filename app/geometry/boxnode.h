#ifndef BOXNODE_H
#define BOXNODE_H

#include "box.h"
#include "node.h"
#include "surface.h"

class BoxNode : public Node
{
public:
    BoxNode();
    float width, depth, heigth;
    Surface * front,  * back,  * left,  * right,  * top,  * bottom;
    QVector<Surface*> surfaces;
    float getWidth();
    float getHeight();
    float getDepth();
    void addPoint(Vector3& pos);
    void draw();
    void drawSelf();

    QVector<Vector3> intersectionPoints(Vector3 from, Vector3 direction);
    float intersectionPoint(Vector3 from, Vector3 direction);
};

#endif // BOXNODE_H
