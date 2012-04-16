#ifndef RIDGENODE_H
#define RIDGENODE_H
#include "basenode.h"
#include "isurfacefeature.h"

class SurfaceNode;

class RidgeNode : public BaseNode, public ISurfaceFeature
{
private:
    Spline baseSpline;
    Spline crossSpline;
    QVector<QVector2D> uv;
    QVector<float> heights;
    SurfaceNode* surfaceNode;
public:
    RidgeNode(RidgeNode& o) : BaseNode(o), baseSpline(o.baseSpline), crossSpline(o.crossSpline), uv(o.uv) {}
    RidgeNode(QVector<QVector2D> uv, SurfaceNode* parent);

    virtual BaseNode * copy() {
        return new RidgeNode(*this);
    }

    void makeWall();

    virtual void repositionOnSurface(SurfaceNode &surfacenode);
    virtual void doTransformSurface(QVector < QVector < Vector3 > > & rows, float resolution, float size);

    QVector<Vector3> intersectionPoints(Vector3 from, Vector3 direction);

    void drawSelf();

    void determineActionOnStoppedDrawing();



};

#endif // RIDGENODE_H
