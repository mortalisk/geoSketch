#ifndef RiverNode_H
#define RiverNode_H
#include "basenode.h"
#include "isurfacefeature.h"

class SurfaceNode;
class Deposit;

class RiverNode : public BaseNode, public ISurfaceFeature
{
    Q_OBJECT
private:
    Spline rightSpline;
    Spline crossSpline;
    bool editLeft;
    bool editRight;
    bool oversketch; // or replace
    Vector3 lastIntersectionFrom;
    Vector3 lastIntersectionDriection;

    // the original sketched line
    QVector<QVector2D> uv;

    //the two river banks
    QVector<QVector2D> lefts;
    QVector<QVector2D> rigths;

    bool drawWater;

    Deposit * deposit;

    QVector<QVector<Vector3> > rows;
    int skip;

QVector<QVector2D> uvSketch;

public:
    RiverNode() {deposit = NULL;}
    RiverNode(RiverNode& o) : BaseNode(o), rightSpline(o.rightSpline), crossSpline(o.crossSpline), uv(o.uv), lefts(o.lefts), rigths(o.rigths), drawWater(o.drawWater), deposit(NULL){}
    RiverNode(QVector<QVector2D> uv, bool drawWater = true, float width = 0.01);

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

    void createDeposit(float seaLevel, SurfaceNode& surfaceNode);

    QString getTypeId() {
        return QString("RiverNode");
    }

    void addSubclassActions(QToolBar *menu);

    void setActive(bool a);

public slots:
    void replace(bool r) {
        oversketch = !r;
    }
};



#endif // RiverNode_H
