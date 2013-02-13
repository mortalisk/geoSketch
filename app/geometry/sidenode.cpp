#include "sidenode.h"
#include "basenode.h"
#include "surface.h"

SideNode::SideNode(Vector3 lowerLeft,Vector3 lowerRigth, Vector3 upperRigth,Vector3 upperLeft) 
    : BaseNode("sideNode"), lowerLeft(lowerLeft), lowerRigth(lowerRigth), upperRigth(upperRigth), upperLeft(upperLeft)
{
    
    init();
  
}

void SideNode::init() {
    QVector<vertex> vertices;

    QVector<Vector3> lineVertices;
    Vector3 normal = (lowerRigth - lowerLeft).cross((upperRigth-lowerLeft)).normalize();
    QVector4D c(1.0, 1.0, 1.0, 0.5);

    //Front
    vertices.push_back(vertex(lowerLeft,normal,0, 0));
    vertices.push_back(vertex(lowerRigth,normal, 1, 0));
    vertices.push_back(vertex(upperLeft,normal, 0, 1));

    vertices.push_back(vertex(upperLeft,normal, 0, 1));
    vertices.push_back(vertex(lowerRigth,normal, 1, 0));
    vertices.push_back(vertex(upperRigth,normal, 1, 1));

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
    const QVector<QVector2D> & points = uvSpline.getPoints();
    QVector2D first = points[0];
    QVector2D last = points[points.size()-1];
    QVector2D left(0,first.y());
    QVector2D right(1,last.y());

    float rdist = (right-last).length();
    float ldist = (left-first).length();
    float dist = (left-right).length();
    float totalPoints = 20.0;
    if (ldist > 0.001) {
        float lPoints = totalPoints*ldist/dist;
        float lInc = ldist/lPoints;
        for (float i = lInc; i< lPoints+lInc; i+=lInc) {
            if (i > lPoints) i= lPoints;
            uvSpline.addPointFront(interpolate(left, first, i/lPoints));
            if (i == lPoints) break;
        }
    }
    if (rdist > 0.001) {
        float rPoints = totalPoints*rdist/dist;
        float rInc = rdist/rPoints;
        for (float i = rInc; i< rPoints+rInc; i+=rInc) {
            if (i > rPoints) i= rPoints;
            uvSpline.addPoint(interpolate(right, last,i/rPoints));
            if (i == rPoints) break;
        }
    }

}

//bool similar(float a, float b, float c) {
//    return fabs(a-b) < 0.001 && fabs(b-c) < 0.001;
//}

void SideNode::addSubclassJson(QVariantMap &map) {
    map["lowerLeft"] = lowerLeft.toJson();
    map["lowerRigth"] = lowerRigth.toJson();
    map["upperLeft"] = upperLeft.toJson();
    map["upperRigth"] = upperRigth.toJson();
}

void SideNode::fromJsonSubclass(QVariantMap map) {
    lowerLeft.fromJson(map);
    lowerRigth.fromJson(map);
    upperLeft.fromJson(map);
    upperRigth.fromJson(map);
    init();
}
