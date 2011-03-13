#include "box.h"

Box::Box(float width, float heigth, float depth, QObject *parent) :
    QObject(parent)
{
}

float Box::getDepth() {
    return 0;
}

float Box::getWidth() {
    return 0;
}

float Box::getHeight() {
    return 0;
}
