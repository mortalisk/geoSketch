#ifndef SURFACE_H
#define SURFACE_H
#include "shape.h"
#include <QVector>
#include "spline.h"

class Surface : public Shape
{
public:
    Surface(QVector<Spline*>& splines);
    virtual QVector<Vector3> intersectionPoints(Vector3 from,Vector3 direction);
};

#endif // SURFACE_H
