#ifndef BOX_H
#define BOX_H

#include <QVector>
#include "shape.h"

class Box : public Shape
{
private:
    float width, heigth, depth;
public:
    Box(float width, float heigth, float depth);
    ~Box() {

    }
    float getWidth();
    float getHeight();
    float getDepth();
};

#endif // BOX_H
