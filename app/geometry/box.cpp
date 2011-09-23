#include "box.h"
#include <morten3d/Vector3.h>
#include <GL/glew.h>

Box::Box(float width, float heigth, float depth) :
    Shape(), width(width),heigth(heigth),depth(depth)
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




