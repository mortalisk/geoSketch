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
    Vector3 upperRigth;
    Vector3 upperLeft;
    Vector3 lowerRigth;
    SideNode(Vector3 lowerLeft,Vector3 lowerRigth, Vector3 upperRigth,Vector3 upperLeft);

    void setOpposite(SideNode * node);
    void setLeft(SideNode * node);
    void projectPoint(Vector3 & point);
    void moveSketchingPointsToSpline();
    void makeSuggestionLines();
};

#endif // SIDENODE_H
