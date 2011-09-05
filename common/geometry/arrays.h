#ifndef ARRAYS_H
#define ARRAYS_H

#include <QObject>
class Shape;

struct vertex {

    vertex(float x = 0, float y = 0, float z = 0, float r = 0, float g = 0, float b = 0, float a = 0) : x(x), y(y), z(z), r(r), g(g), b(b), a(a) {

    }

    float x,y,z;
    float r,g,b,a;
};


#endif // ARRAYS_H
