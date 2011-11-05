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
    Node();
    Node(Shape * shape);
    virtual ~Node();
    QVector<Node*> children;
    Spline spline;
    Spline sketchingSpline;
    bool drawing;
    bool splineDone;

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

protected:
private:
    int findNearestPointInSpline(Vector3 first);

};

#endif // NODE_H
