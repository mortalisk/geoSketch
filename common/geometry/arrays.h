#ifndef ARRAYS_H
#define ARRAYS_H

#include <QObject>
class Shape;

struct vertex {
    float x,y,z;
    float r,g,b,a;
};

class Arrays {

public:

    static vertex* makeArray(Shape& shape, int& size);

};

#endif // ARRAYS_H
