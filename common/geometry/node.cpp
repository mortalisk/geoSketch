#include "node.h"

Node::Node()
{
    foreach(Spline* s , splines) {
        delete s;
    }
}

QVector<Vector3> Node::intersectionPoints(Vector3 from,Vector3 direction) {
    from = from - position;
    direction = direction - position;
    return shape->intersectionPoints(from, direction);
}
