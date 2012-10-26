#ifndef VALLEYNODE_H
#define VALLEYNODE_H
#include "basenode.h"
#include "isurfacefeature.h"

class SurfaceNode;
class Deposit;

class ValleyNode : public BaseNode, public ISurfaceFeature
{
private:
    Spline rightSpline;
    Spline crossSpline;

    // the original sketched line
    QVector<QVector2D> uv;

    //the two river banks
    QVector<QVector2D> lefts;
    QVector<QVector2D> rigths;

    Deposit * deposit;

QVector<QVector2D> uvSketch;

public:
    ValleyNode() {}
    ValleyNode(ValleyNode& o) : BaseNode(o), rightSpline(o.rightSpline), crossSpline(o.crossSpline), uv(o.uv), lefts(o.lefts), rigths(o.rigths), deposit(NULL) {}
    ValleyNode(QVector<QVector2D> uv);

    virtual BaseNode * copy() {
        return new ValleyNode(*this);
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

    void createDeposit(float seaLevel, SurfaceNode& surfaceNode);

    QString getTypeId() {
        return QString("ValleyNode");
    }

};




#endif // VALLEYNODE_H
