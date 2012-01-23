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
    Vector3 left(lowerLeft.x(),first.y(),lowerLeft.z());
    Vector3 right(lowerRigth.x(),last.y(),lowerRigth.z());

    float rdist = (right-last).lenght();
    float ldist = (left-first).lenght();
    float dist = (left-right).lenght();
    float totalPoints = 20.0;
    if (ldist > 0.001) {
        float lPoints = totalPoints*ldist/dist;
        float lInc = ldist/lPoints;
        for (float i = lInc; i< lPoints+lInc; i+=lInc) {
            if (i > lPoints) i= lPoints;
            spline.points.push_front(interpolate(left, first, i/lPoints));
            if (i == lPoints) break;
        }
    }
    if (rdist > 0.001) {
        float rPoints = totalPoints*rdist/dist;
        float rInc = rdist/rPoints;
        for (float i = rInc; i< rPoints+rInc; i+=rInc) {
            if (i > rPoints) i= rPoints;
            spline.points.push_back(interpolate(right, last,i/rPoints));
            if (i == rPoints) break;
        }
    }

}

static bool similar(float a, float b, float c) {
    return fabs(a-b) < 0.001 && fabs(b-c) < 0.001;
}
