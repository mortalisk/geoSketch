#ifndef RIDGENODE_H
#define RIDGENODE_H
#include "basenode.h"
#include "isurfacefeature.h"

class SurfaceNode;

class RidgeNode : public BaseNode, ISurfaceFeature
{
public:
    RidgeNode(RidgeNode& o) : BaseNode(o) {}
    RidgeNode(Spline& spline);

    virtual BaseNode * copy() {
        return new RidgeNode(*this);
    }

    void makeWall();

    virtual void repositionOnSurface(SurfaceNode &surfacenode);
    virtual void doTransformSurface(SurfaceNode &surfacenode);


private:
    Spline baseSpline;
};

#endif // RIDGENODE_H
