#ifndef NODE_H
#define NODE_H
#include <QVector>
#include "morten3d/Vector3.h"
#include "shape.h"
#include "spline.h"

class Node
{
public:
    Shape * shape;
    Vector3 position;
    QVector<Node*> children;
    Spline spline;
    Spline sketchingSpline;
    bool drawing;
    bool splineDone;
    bool visible;
    QString name;

    QVector4D diffuse;
    QVector4D ambient;

    Node(QString name);
    Node(Shape * shape, QString name);
    Node(Node &other)
        : shape(other.shape),position(other.position),
        spline(other.spline),sketchingSpline(other.sketchingSpline),
        drawing(other.drawing),splineDone(other.splineDone),visible(other.visible), diffuse(other.diffuse), ambient(other.ambient)
    {
        name = "copy of: " + other.name;
        foreach(Node* child, children) {
            children.push_back(child->copy());
        }
    }
    virtual ~Node();
    virtual Node* copy() {
        Node * node = new Node(*this);
        return node;
    }



    void addChild(Node* child) {
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

    virtual Node * makeLayer();


    void drawSplines();
    void drawSpline(Spline & spline, float r);


    void correctSketchingDirection();

    void doOversketch();
    void oversketchSide(Vector3& pointInSketch, int nearest, bool first);
    virtual bool isPointNearerSide(Vector3& point, int indexInSpline);

    void moveSketchingPointsToSpline();

};

#endif // NODE_H
