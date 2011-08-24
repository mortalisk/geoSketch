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
    QVector<Node*> children;
    QVector<Spline*> splines;
    bool drawing;
    QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);
    void addSpline() {
        splines.append(new Spline());
    }


};

#endif // NODE_H
