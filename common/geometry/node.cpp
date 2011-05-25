#include "node.h"

Node::Node()
{
}

QVector<Vector3> Node::intersectionPoints(Vector3 from,Vector3 direction) {
    from = from - position;
    direction = direction - position;
    return shape->intersectionPoints(from, direction);
}
