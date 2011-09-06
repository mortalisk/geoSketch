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
    QVector<vertex> & getVertices() {
        return vertices;
    }
signals:

public slots:

};

#endif // SHAPE_H
