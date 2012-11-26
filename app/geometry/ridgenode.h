#ifndef RIDGENODE_H
#define RIDGENODE_H
#include "basenode.h"
#include "isurfacefeature.h"

class SurfaceNode;

class RidgeNode : public BaseNode, public ISurfaceFeature
{
    Q_OBJECT
private:
    Spline baseSpline;
    QVector<QVector2D> uv;
    QVector<float> heights;
    QVector<QVector2D> uvSketch;
public:
    RidgeNode() {}
    RidgeNode(RidgeNode& o) : BaseNode(o), baseSpline(o.baseSpline), uv(o.uv), heights(o.heights), uvSketch(o.uvSketch) {}
    RidgeNode(QVector<QVector2D> uv);
    virtual BaseNode * copy() {
        return new RidgeNode(*this);
    }

    void smooth(QVector<QVector2D>& uv);

    void makeWall();

    void addPoint(Vector3 from, Vector3 direction);

    virtual void repositionOnSurface(SurfaceNode &surfacenode);
    virtual void doTransformSurface(QVector < QVector < Vector3 > > & rows, float resolution, float size);

    QVector<Vector3> intersectionPoints(Vector3 from, Vector3 direction);

    void draw();

    void drawSelf();

    void determineActionOnStoppedDrawing();

    void addSubclassJson(QVariantMap &map);

    void fromJsonSubclass(QVariantMap map);

    QString getTypeId() {
        return QString("RidgeNode");
    }

    void addSubclassActions(QToolBar *){};



};

#endif // RIDGENODE_H
