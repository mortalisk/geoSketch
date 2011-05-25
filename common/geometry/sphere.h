#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"
#include <QVector>

class Sphere : public Shape
{
    float radius;
public:
    Sphere(float r);
};

#endif // SPHERE_H
