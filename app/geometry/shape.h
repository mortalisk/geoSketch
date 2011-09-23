#ifndef SHAPE_H
#define SHAPE_H

#include <morten3d/Vector3.h>
#include "arrays.h"
#include <QVector>

class Shape
{
public:
    Shape();
    virtual void drawLines();
    virtual void drawShape();
    virtual QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);
protected:
    int displayList;
    Vector3 color;
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
