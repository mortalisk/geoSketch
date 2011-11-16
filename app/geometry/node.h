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

    Node();
    Node(Shape * shape);
    Node(Node &other)
        : shape(other.shape),position(other.position),
        spline(other.spline),sketchingSpline(other.sketchingSpline),
        drawing(other.drawing),splineDone(other.splineDone)
    {
        QVector<Node*> newChildren;
        foreach(Node* child, children) {
            Node * newChild = child->copy();
            newChildren.push_back(newChild);
        }
        children.clear();
        children += newChildren;
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

    virtual void makeLayer();


    void drawSplines();
    void drawSpline(Spline & spline);

    void doOversketch();

};

#endif // NODE_H
