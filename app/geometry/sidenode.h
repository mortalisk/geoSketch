#ifndef SIDENODE_H
#define SIDENODE_H
#include "shape.h"
#include "node.h"

class SideNode : public Node
{
public:
    SideNode * opposite;
    SideNode * left;
    SideNode * right;
    Vector3 lowerLeft;
    Vector3 lowerRigth;
    Vector3 upperRigth;
    Vector3 upperLeft;
    SideNode(Vector3 lowerLeft,Vector3 lowerRigth, Vector3 upperRigth,Vector3 upperLeft);
    SideNode(SideNode& o) :Node::Node(o), lowerLeft(o.lowerLeft), lowerRigth(o.lowerRigth), upperRigth(o.upperRigth), upperLeft(o.upperLeft) {

    }

    void setOpposite(SideNode * node);
    void setLeft(SideNode * node);
    void projectPoint(Vector3 & point);
    void moveSketchingPointsToSpline();
    void makeSuggestionLines();
    SideNode * copy() {
        SideNode * node = new SideNode(*this);
        return node;
    }
};

#endif // SIDENODE_H
