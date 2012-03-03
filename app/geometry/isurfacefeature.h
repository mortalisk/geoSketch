#ifndef ISURFACEFEATURE_H
#define ISURFACEFEATURE_H
#include "surfacenode.h"
class ISurfaceFeature
{
public:
    ISurfaceFeature();
    virtual void doTransformSurface(SurfaceNode & surfacenode) = 0;
    virtual void repositionOnSurface(SurfaceNode & surfacenode) = 0;
};

#endif // ISURFACEFEATURE_H
