#ifndef SURFACENODE_H
#define SURFACENODE_H
#include "basenode.h"
#include "spline.h"
class SurfaceNode : public BaseNode
{
    Spline front, right, back, left;
    SurfaceNode* below;
    // this vairable enables contruction of triangles before drawing
    // in stead of when copying which made interaction laggy
    bool hasContructedLayer;
public:
    SurfaceNode(QString name, Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below = NULL);
    SurfaceNode(SurfaceNode& other);
    void constructLayer();
    virtual BaseNode* copy();

    void drawWall();

    void drawSelf();

    virtual void prepareForDrawing();

    virtual void determineActionOnStoppedDrawing();

    void makeRidgeNode();
};

#endif // SURFACENODE_H
