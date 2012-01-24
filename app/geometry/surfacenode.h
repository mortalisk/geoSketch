#ifndef SURFACENODE_H
#define SURFACENODE_H
#include "node.h"
#include "spline.h"
class SurfaceNode : public Node
{
    Spline front, right, back, left;
    SurfaceNode* below;
public:
    SurfaceNode(QString name, Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below = NULL);
    void constructLayer();
};

#endif // SURFACENODE_H
