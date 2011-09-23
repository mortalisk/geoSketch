#ifndef SURFACE_H
#define SURFACE_H
#include "shape.h"
#include <QVector>
#include <QVector4D>
#include "spline.h"
#include "arrays.h"

class Surface : public Shape
{
public:
    /** takes a list of vertices that reprecent trinagles of surfaces*/
    Surface(QVector<Vector3> & triangles, QVector<Vector3> & outline, QVector4D &color);
    Surface(QVector<Spline*>& splines, QVector4D &color);
};

#endif // SURFACE_H
