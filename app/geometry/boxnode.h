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
    BoxNode(BoxNode& o);

    ~BoxNode() {
        foreach(SideNode* p, surfaces) {
            delete p;
        }
    }

    void setUpSurfaces();

    float width, depth, heigth;
    SideNode * frontNode,  * backNode,  * leftNode,  * rightNode,  * topNode,  * bottomNode;
    float topF, bottomF ,rightF ,leftF ,farF , nearF;
    QVector<SideNode*> surfaces;
    SideNode * activeSurface;
    float getWidth();
    float getHeight();
    float getDepth();
    void addPoint(Vector3 from, Vector3 direction);
    void determineActionOnStoppedDrawing();
    void draw();
    void drawSelf();
    void makeLayer();
    void makeSuggestionFor(SideNode* side);

    QVector<Vector3> intersectionPoints(Vector3 from, Vector3 direction);
    float intersectionPoint(Vector3 from, Vector3 direction);

    BoxNode * copy() {
        BoxNode * node = new BoxNode(*this);


        return node;
    }
};

#endif // BOXNODE_H
