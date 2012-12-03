#ifndef SHAPE_H
#define SHAPE_H

#include <morten3d/Vector3.h>
#include "arrays.h"
#include <QVector>

class Shape
{
protected:
    int displayList;
    QVector<vertex> lineVertices;

public:
bool strip;
    QVector<vertex> triangles;
    Shape();
    ~Shape();
    void drawLines(bool stipple, float red, float green, float blue, float alpha);
    void drawShape(QVector4D ambient, QVector4D diffuse);
    QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction,float &s, float &t);

};

#endif // SHAPE_H
