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
    SideNode(Shape * shape);

    void setOpposite(SideNode * node);
    void setLeft(SideNode * node);
    void projectPoint(Vector3 & point);
};

#endif // SIDENODE_H
