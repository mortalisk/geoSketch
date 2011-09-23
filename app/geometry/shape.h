#ifndef SHAPE_H
#define SHAPE_H

#include <morten3d/Vector3.h>
#include "arrays.h"
#include <QVector>

class Shape
{
public:
    Shape();
    virtual void draw();
    virtual QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);
protected:
    int displayList;
    QVector<vertex> vertices;
    QVector<vertex> lineVertices;
    QVector<vertex> & getVertices() {
        return vertices;
    }
    QVector<vertex> & getLineVertices() {
        return lineVertices;
    }
signals:

public slots:

};

#endif // SHAPE_H
