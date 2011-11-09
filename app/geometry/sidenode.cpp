#include "sidenode.h"
#include "node.h"
#include "surface.h"

SideNode::SideNode(Vector3 lowerLeft,Vector3 lowerRigth, Vector3 upperRigth,Vector3 upperLeft) 
  : lowerLeft(lowerLeft), lowerRigth(lowerRigth), upperRigth(upperRigth), upperLeft(upperLeft)
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

    shape = new Surface(vertices, lineVertices, c);  
  
}

void SideNode::setOpposite(SideNode * node) {
    this->opposite = node;
    node->opposite = this;
}

void SideNode::setLeft(SideNode *node) {
    this->left = node;
    node->right = this;
}

void SideNode::moveSketchingPointsToSpline() {
   spline.points += sketchingSpline.points;
   sketchingSpline.points.clear();
}

void SideNode::makeSuggestionLines() {

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
