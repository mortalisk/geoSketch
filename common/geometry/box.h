#ifndef BOX_H
#define BOX_H

#include <QVector>
#include "shape.h"

class Box : public Shape
{
private:
    float width, heigth, depth;
    QVector<vertex> vertices;
public:
    Box(float width, float heigth, float depth);
    float getWidth();
    float getHeight();
    float getDepth();

    virtual QVector<vertex> & getVertices();

signals:

public slots:

};

#endif // BOX_H
