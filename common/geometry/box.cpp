#include "box.h"

Box::Box(float width, float heigth, float depth, QObject *parent) :
    QObject(parent), width(width),heigth(heigth),depth(depth)
{
}

float Box::getDepth() {
    return depth;
}

float Box::getWidth() {
    return width;
}

float Box::getHeight() {
    return heigth;
}
