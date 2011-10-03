#ifndef BOXNODE_H
#define BOXNODE_H

#include "box.h"
#include "node.h"
#include "surface.h"
#include "sidenode.h"

class BoxNode : public Node
{
public:
    BoxNode();
    float width, depth, heigth;
    SideNode * frontNode,  * backNode,  * leftNode,  * rightNode,  * topNode,  * bottomNode;
    //QVector<Surface*> surfaces;
    QVector<SideNode*> surfaces;
    SideNode * activeSurface;
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
