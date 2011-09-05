#include "boxnode.h"

#include <iostream>
#include "GL/glew.h"

BoxNode::BoxNode()
{
    box = new Box(10,10,10);
    shape = box;
}

float BoxNode::getDepth() {
    return box->getDepth();
}

float BoxNode::getWidth() {
    return box->getWidth();
}

float BoxNode::getHeight() {
    return box->getHeight();
}

 void BoxNode::addPoint(Vector3& pos) {
     std::cout << "BoxNode: going to add point" << std::endl;
    if (splines.size() == 0) {
        addSpline();
    }

    Spline* spline = splines[splines.size()-1];


    spline->addPoint(pos);

 }

 void BoxNode::draw() {
     glDisable(GL_CULL_FACE);
     drawChildren();
     glEnable(GL_CULL_FACE);
     drawSelf();
     glDisable(GL_CULL_FACE);
 }
