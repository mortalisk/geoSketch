#ifndef RiverNode_H
#define RiverNode_H
#include "basenode.h"
#include "isurfacefeature.h"

class SurfaceNode;

class RiverNode : public BaseNode, public ISurfaceFeature
{
private:
    Spline rightSpline;
    Spline crossSpline;

    // the original sketched line
    QVector<QVector2D> uv;

    //the two river banks
    QVector<QVector2D> lefts;
    QVector<QVector2D> rigths;

QVector<QVector2D> uvSketch;

    SurfaceNode* surfaceNode;
public:
    RiverNode() {}
    RiverNode(RiverNode& o) : BaseNode(o), rightSpline(o.rightSpline), crossSpline(o.crossSpline), uv(o.uv) {}
    RiverNode(QVector<QVector2D> uv, SurfaceNode* parent);

    virtual BaseNode * copy() {
        return new RiverNode(*this);
    }

    void smooth(QVector<QVector2D> &);

    void makeWater();

    void doOversketch();

    virtual void repositionOnSurface(SurfaceNode &surfacenode);
    virtual void doTransformSurface(QVector < QVector < Vector3 > > & rows, float resolution, float size);

    QVector<Vector3> intersectionPoints(Vector3 from, Vector3 direction);

    void drawSelf();
    void drawSplines();

    void determineActionOnStoppedDrawing();

    void removeLoops(QVector<QVector2D>& sp);

    void addPoint(Vector3 from, Vector3 direction);

    void addSubclassJson(QVariantMap &map);

    void fromJsonSubclass(QVariantMap map);

    QString getTypeId() {
        return QString("RiverNode");
    }

};

#endif // RiverNode_H
