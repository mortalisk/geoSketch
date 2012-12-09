#ifndef ISURFACEFEATURE_H
#define ISURFACEFEATURE_H
#include <QVector>
#include "Vector3.h"
class SurfaceNode;
class ISurfaceFeature
{
public:
    ISurfaceFeature(){}
    virtual void doTransformSurface(QVector < QVector < Vector3 > > & rows, float resolution, float size) = 0;
    virtual void repositionOnSurface(SurfaceNode & surfacenode) = 0;
    virtual float dist(float s, float t) = 0;
};

#endif // ISURFACEFEATURE_H
