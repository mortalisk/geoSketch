#include "sidenode.h"
#include "node.h"
#include "surface.h"

SideNode::SideNode(Vector3 lowerLeft,Vector3 lowerRigth, Vector3 upperRigth,Vector3 upperLeft) 
    : Node::Node("sideNode"), lowerLeft(lowerLeft), lowerRigth(lowerRigth), upperRigth(upperRigth), upperLeft(upperLeft)
{
    
    QVector<Vector3> vertices;
    QVector<Vector3> lineVertices;
    
    QVector4D c(1.0, 1.0, 1.0, 0.5);

    //Front
    vertices.push_back(lowerLeft);
    vertices.push_back(lowerRigth);
    vertices.push_back(upperLeft);

    vertices.push_back(upperLeft);
    vertices.push_back(lowerRigth);
    vertices.push_back(upperRigth);

    lineVertices.push_back(lowerLeft);
    lineVertices.push_back(lowerRigth);
    lineVertices.push_back(upperRigth);
    lineVertices.push_back(upperLeft);
    lineVertices.push_back(lowerLeft);

    shape = new Surface(vertices, lineVertices);
  
}

void SideNode::setOpposite(SideNode * node) {
    this->opposite = node;
    node->opposite = this;
}

void SideNode::setLeft(SideNode *node) {
    this->left = node;
    node->right = this;
}

void SideNode::makeSuggestionLines() {

    ensureLeftToRigth();
    Vector3 first = spline.points[0];
    Vector3 last = spline.points[spline.points.size()-1];

    for (float i = 0.1; i< 1.01; i+=0.1) {
        spline.points.push_front(interpolate(Vector3(lowerLeft.x(),first.y(),lowerLeft.z()), first, i));

    }
    for (float i = 0.1; i< 1.01; i+=0.1) {
        spline.points.push_back(interpolate(Vector3(lowerRigth.x(),last.y(),lowerRigth.z()), last,i));
    }

}

static bool similar(float a, float b, float c) {
    return fabs(a-b) < 0.001 && fabs(b-c) < 0.001;
}
