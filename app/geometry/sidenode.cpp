#include "sidenode.h"
#include "node.h"

SideNode::SideNode(Shape * shape) : Node(shape)
{

}

void SideNode::setOpposite(SideNode * node) {
    this->opposite = node;
    node->opposite = this;
}

void SideNode::setLeft(SideNode *node) {
    this->left = node;
    node->right = this;
}

static bool similar(float a, float b, float c) {
    return fabs(a-b) < 0.001 && fabs(b-c) < 0.001;
}

void SideNode::projectPoint(Vector3 & point) {
    if (similar(shape->triangles[1].p1.x,shape->triangles[1].p2.x,shape->triangles[1].p3.x)) {
        Vector3 p(shape->triangles[1].p1.x,point.y(), point.z());
        spline.addPoint(p);
    } else if (similar(shape->triangles[1].p1.y,shape->triangles[1].p2.y,shape->triangles[1].p3.y)) {
        Vector3 p(point.x(),shape->triangles[1].p1.y, point.z());
        spline.addPoint(p);
    } else if (similar(shape->triangles[1].p1.z,shape->triangles[1].p2.z,shape->triangles[1].p3.z)) {
        Vector3 p(point.x(),point.y(),shape->triangles[1].p1.z);
        spline.addPoint(p);
    }
    spline.isSuggestion = true;
}

void SideNode::stopDrawing() {
    if (spline.points.size() == 0) {
        spline.points += sketchingSpline.points;
        sketchingSpline.points.clear();
    } else {
        doOversketch();
    }
}
