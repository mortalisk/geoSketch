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
    virtual ~Node();
    QVector<Node*> children;
    QVector<Spline*> splines;
    bool drawing;

    virtual QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);

    virtual void addPoint(Vector3& pos);

    virtual void draw();

    void drawSelf();
    void drawChildren();

    virtual void makeLayer();

protected:
    void addSpline();

};

#endif // NODE_H
