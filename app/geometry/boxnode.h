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
    Node * front,  * back,  * left,  * right,  * top,  * bottom;
    //QVector<Surface*> surfaces;
    QVector<Node*> surfaces;
    Node * activeSurface;
    float getWidth();
    float getHeight();
    float getDepth();
    void addPoint(Vector3 from, Vector3 direction);
    void stopDrawing();
    void draw();
    void drawSelf();

    QVector<Vector3> intersectionPoints(Vector3 from, Vector3 direction);
    float intersectionPoint(Vector3 from, Vector3 direction);
};

#endif // BOXNODE_H
