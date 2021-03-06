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
    /** takes a list of vertices that reprecent trinagles of surfaces, and one normal for each vertice*/
    Surface(QVector<vertex> & triangles, QVector<Vector3> & outline,bool strip = false);
};

#endif // SURFACE_H
