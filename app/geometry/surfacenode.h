#ifndef SURFACENODE_H
#define SURFACENODE_H
#include "basenode.h"
#include "spline.h"
#include "isurfacefeature.h"
class SurfaceNode : public BaseNode
{
    Spline front, right, back, left;
    SurfaceNode* below;
    // this vairable enables contruction of triangles before drawing
    // in stead of when copying which made interaction laggy
    bool hasContructedLayer;

    void makeSide(Spline& belowSpline, Spline& spline,QVector<Vector3>& normals, QVector<Vector3>& triangles);
public:
    void invalidate();
    SurfaceNode(QString name, Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below = NULL);
    SurfaceNode(SurfaceNode& other);
    void constructLayer();
    virtual BaseNode* copy();

    virtual void prepareForDrawing();

    virtual void determineActionOnStoppedDrawing();

    void makeRidgeNode();

    void drawChildren();

    QVector<Vector3> triangulatePolygon(QVector<Vector3> vertices);
};

#endif // SURFACENODE_H
