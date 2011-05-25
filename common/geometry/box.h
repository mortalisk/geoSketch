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
    float getWidth();
    float getHeight();
    float getDepth();

signals:

public slots:

};

#endif // BOX_H
