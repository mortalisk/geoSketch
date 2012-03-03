#ifndef SHAPE_H
#define SHAPE_H

#include <morten3d/Vector3.h>
#include "arrays.h"
#include <QVector>

class Shape
{
public:
    Shape();
    ~Shape();
    void drawLines(bool stipple);
    void drawShape(QVector4D ambient, QVector4D diffuse);
    QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);

    int displayList;
    QVector<triangle> triangles;
    QVector<vertex> lineVertices;
    QVector<triangle> & getTriangles() {
        return triangles;
    }
    QVector<vertex> & getLineVertices() {
        return lineVertices;
    }
};

#endif // SHAPE_H
