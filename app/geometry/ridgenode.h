#ifndef RIDGENODE_H
#define RIDGENODE_H
#include "basenode.h"

class SurfaceNode;

class RidgeNode : public BaseNode
{
public:
    RidgeNode(RidgeNode& o) : BaseNode(o) {}
    RidgeNode(Spline& spline);

    virtual BaseNode * copy() {
        return new RidgeNode(*this);
    }
};

#endif // RIDGENODE_H
