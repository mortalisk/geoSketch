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
    float resolution;

    QVector<QVector2D> uvCoordinateSpline;
    QVector < QVector < Vector3 > > rows;
    void makeSide(Spline& belowSpline, Spline& spline,QVector<Vector3>& normals, QVector<Vector3>& triangles);
public:
    void invalidate();
    SurfaceNode(QString name, Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below = NULL);
    SurfaceNode(SurfaceNode& other);
    void constructLayer();
    virtual BaseNode* copy();

    Vector3 getPointFromUv(QVector2D uv);

    virtual void prepareForDrawing();

    virtual void determineActionOnStoppedDrawing();

    void makeRidgeNode();

    void drawChildren();

    void addPoint(Vector3 from, Vector3 direction);
};

#endif // SURFACENODE_H
