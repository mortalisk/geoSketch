#include "ridgenode.h"
#include "surfacenode.h"

RidgeNode::RidgeNode(Spline& spline) : BaseNode("hei")
{
    this->spline.points += spline.points;
}
