#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"
#include <QVector>

class Sphere : public Shape
{
    float radius;
public:
    Sphere(float radius, float r, float g, float b);
};

#endif // SPHERE_H
