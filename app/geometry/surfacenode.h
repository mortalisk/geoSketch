#ifndef SURFACENODE_H
#define SURFACENODE_H
#include "basenode.h"
#include "spline.h"
#include "isurfacefeature.h"
class SurfaceNode : public BaseNode
{
    SurfaceNode* below;
    // this vairable enables contruction of triangles before drawing
    // in stead of when copying which made interaction laggy
    bool hasContructedLayer;
    int resolution;
    int skip;

    QVector<QVector2D> uvCoordinateSpline;
    QVector < QVector < Vector3 > > rows;
    QVector < QVector < Vector3 > > intersectRows;
    void makeSide(Spline& belowSpline, Spline& spline,QVector<Vector3>& normals, QVector<Vector3>& triangles);
public:

    Spline front, right, back, left;
    void invalidate();
    SurfaceNode(QString name, Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below = NULL);
    SurfaceNode(SurfaceNode& other);
    void constructLayer();
    virtual BaseNode* copy();

    Vector3 getPointFromUv(QVector2D uv);

    virtual void prepareForDrawing();

    virtual void determineActionOnStoppedDrawing();

    void makeRiverNode();

    void makeRidgeNode();

    void drawChildren();

    void addPoint(Vector3 from, Vector3 direction);

    virtual QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);
};

#endif // SURFACENODE_H
