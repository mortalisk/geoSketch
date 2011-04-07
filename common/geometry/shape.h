#ifndef SHAPE_H
#define SHAPE_H

#include <QVector3D>
#include "arrays.h"

class Shape
{
public:
    Shape();
    vertex* array;
    int arraySize;

    virtual QVector<vertex> & getVertices() = 0;

signals:

public slots:

};

#endif // SHAPE_H
