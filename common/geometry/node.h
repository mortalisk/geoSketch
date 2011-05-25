#ifndef NODE_H
#define NODE_H
#include <QVector>
#include "morten3d/Vector3.h"
#include "shape.h"

class Node
{
public:
    Shape * shape;
    Vector3 position;
    Node();
    QVector<Node*> children;
    QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);

    void draw();

};

#endif // NODE_H
