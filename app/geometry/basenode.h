#ifndef NODE_H
#define NODE_H
#include <QVector>
#include "morten3d/Vector3.h"
#include "shape.h"
#include "spline.h"

class BaseNode
{
public:
    Shape * shape;
    Vector3 position;
    QVector<BaseNode*> children;
    Spline spline;
    Spline sketchingSpline;
    bool drawing;
    bool splineDone;
    bool visible;
    QString name;

    BaseNode * parent;

    QVector4D diffuse;
    QVector4D ambient;

    BaseNode(QString name);
    BaseNode(Shape * shape, QString name);
    BaseNode(BaseNode &other)
        : shape(other.shape),position(other.position),
        spline(other.spline),sketchingSpline(other.sketchingSpline),
        drawing(other.drawing),splineDone(other.splineDone),visible(other.visible), diffuse(other.diffuse), ambient(other.ambient)
    {
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

    virtual QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);

    /** adds a point to currend spline */
    virtual void addPoint(Vector3 from, Vector3 direction);
    /** stops drawing on current spline */
    virtual void determineActionOnStoppedDrawing();

    virtual void draw();

    virtual void drawSelf();
    void drawChildren();
    virtual void prepareForDrawing() {

    }

    virtual BaseNode * makeLayer();


    void drawSplines();
    void drawSpline(Spline & spline, float r);


    void correctSketchingDirection();

    void doOversketch();
    void oversketchSide(Vector3& pointInSketch, int nearest, bool first);
    virtual bool isPointNearerSide(Vector3& point, int indexInSpline);

    void moveSketchingPointsToSpline();

};

#endif // NODE_H
