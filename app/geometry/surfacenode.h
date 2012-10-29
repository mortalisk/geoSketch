#ifndef SURFACENODE_H
#define SURFACENODE_H
#include "basenode.h"
#include "spline.h"
#include "isurfacefeature.h"
class SurfaceNode : public BaseNode
{
    Q_OBJECT

    // this vairable enables contruction of triangles before drawing
    // in stead of when copying which made interaction laggy
    bool hasContructedLayer;

    QVector<QVector2D> uvCoordinateSpline;

    QVector<QVector2D> uvSketchingSpline;

    QVector<QVector2D> uvSmoothed;
    QVector < QVector < Vector3 > > intersectRows;
    void makeSide(Spline& belowSpline, Spline& spline, QVector<vertex>& triangles, QVector<Vector3>& outline);
public:
    SurfaceNode* below;
    Spline front, right, back, left;
    QVector < QVector < Vector3 > > rows;
    int resolution;
    int skip;
    void invalidate();
    SurfaceNode() {}
    SurfaceNode(QString name, Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below = NULL);
    SurfaceNode(SurfaceNode& other);
    void constructLayer();
    virtual BaseNode* copy();

    Vector3 getPointFromUv(QVector2D uv);

    virtual void prepareForDrawing();

    virtual void determineActionOnStoppedDrawing();




    void drawChildren();

    void addPoint(Vector3 from, Vector3 direction);

    virtual QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);

    void addSubclassJson(QVariantMap &map);

    void fromJsonSubclass(QVariantMap map);

    void elevateTo(float y);

    QString getTypeId() {
        return QString("SurfaceNode");
    }

    void smoothSketchUv();

    void addSubclassActions(QMenu *menu);
    static QVector<Vector3> intersectionOnRows(Vector3& from, Vector3& direction, QVector<QVector<Vector3> > & rows, float &s, float&t, int skip);
public slots:
    void makeRiverNode();
    void makeRidgeNode();

    void makeValleyNode();
};

#endif // SURFACENODE_H
