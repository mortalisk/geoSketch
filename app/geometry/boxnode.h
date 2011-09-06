#ifndef BOXNODE_H
#define BOXNODE_H

#include "box.h"
#include "node.h"

class BoxNode : public Node
{
public:
    BoxNode();
    Box * box;

    float getWidth();
    float getHeight();
    float getDepth();
    void addPoint(Vector3& pos);
    void draw();
};

#endif // BOXNODE_H
