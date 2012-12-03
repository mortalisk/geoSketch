#ifndef NODE_H
#define NODE_H
#include <QVector>
#include <QVector2D>
#include "morten3d/Vector3.h"
#include "shape.h"
#include "spline.h"
#include <QVariantMap>
#include <QAction>
#include <QToolBar>

class QTextStream;

class Scene;

class BaseNode : public QObject
{
    Q_OBJECT
protected:
    bool active;
public:
    Shape * shape;
    Vector3 position;
    QVector<BaseNode*> children;
    Spline spline;
    Spline sketchingSpline;
    QVector<QVector2D> uvSpline;
    QVector<QVector2D> uvSketchingSpline;

    bool drawing;
    bool splineDone;
    bool visible;
    QString name;

    void init();

    BaseNode * parent;

    BaseNode * proxy;

    QVector4D diffuse;
    QVector4D ambient;

    BaseNode() { init(); }
    BaseNode(QString name);
    BaseNode(Shape * shape, QString name);
    BaseNode(BaseNode &other)
        : QObject(), shape(other.shape),position(other.position),
        spline(other.spline),sketchingSpline(other.sketchingSpline),
          drawing(other.drawing),splineDone(other.splineDone),visible(other.visible),proxy(NULL), diffuse(other.diffuse), ambient(other.ambient)
    {
        init();
        name = other.name;
        foreach(BaseNode* child, other.children) {
            BaseNode * c = child->copy();
            c->parent = this;
            children.push_back(c);
        }
    }
    virtual ~BaseNode();
    virtual BaseNode* copy() = 0;

    void addChild(BaseNode* child) {
        children.push_back(child);
    }

    bool isActive() {
        return active;
    }


    virtual void setActive(bool a) {
        if (a == false) {
            foreach(BaseNode * child, children) {
                child->setActive(a);
            }
        }
        active = a;
    }

    BaseNode * findIntersectingNode(Vector3& from, Vector3& direction, Vector3& point);

    virtual QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);

    /** adds a point to currend spline */
    virtual void addPoint(Vector3 from, Vector3 direction);
    /** stops drawing on current spline */
    virtual void determineActionOnStoppedDrawing();

    virtual void draw();

    virtual void drawSelf();
    virtual void drawChildren();
    virtual void prepareForDrawing() {

    }



    virtual void drawSplines();
    void drawSpline(Spline & spline, float r);


    void correctSketchingDirection();

    void doOversketch();
    void oversketchSide(Vector3& pointInSketch, int nearest, bool first);
    virtual bool isPointNearerSide(Vector3& point, int indexInSpline);

    void moveSketchingPointsToSpline();

    QVariantMap toJson();
    virtual QString getTypeId() = 0;
    virtual void addSubclassJson(QVariantMap& map) = 0;
    void fromJson(QVariantMap map);
    virtual void fromJsonSubclass(QVariantMap map) = 0;

    void addActions(QToolBar * menu);
    virtual void addSubclassActions(QToolBar *) = 0;

    virtual bool includeShapeInExport() = 0;

    void makeObj(QTextStream&, QTextStream &materials, int *objNo);

    void deleteChild(BaseNode * child);

    virtual void childDeleted(BaseNode * child) {
        parent->childDeleted(child);
    }

    virtual void invalidate() {}
public slots:
    void deleteItem() {
        if (parent)
            parent->deleteChild(this);
    }

    virtual void clearSketch() {
        spline.clear();
        sketchingSpline.clear();
        uvSpline.clear();
        uvSketchingSpline.clear();
    }


};

#endif // NODE_H
