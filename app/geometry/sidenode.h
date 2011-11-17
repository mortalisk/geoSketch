#ifndef SIDENODE_H
#define SIDENODE_H
#include "shape.h"
#include "node.h"

class SideNode : public Node
{
public:
    SideNode * opposite;
    SideNode * left;
    SideNode * right;
    Vector3 lowerLeft;
    Vector3 lowerRigth;
    Vector3 upperRigth;
    Vector3 upperLeft;
    SideNode(Vector3 lowerLeft,Vector3 lowerRigth, Vector3 upperRigth,Vector3 upperLeft);
    SideNode(SideNode& o) :Node::Node(o), lowerLeft(o.lowerLeft), lowerRigth(o.lowerRigth), upperRigth(o.upperRigth), upperLeft(o.upperLeft) {

    }

    bool isPointNearerSide(Vector3 &point, int indexInSpline) {
        Vector3 leftSide(lowerLeft.x(), point.y(), lowerLeft.z());
        Vector3 rightSide(lowerRigth.x(), point.y(), lowerRigth.z());
        Vector3& inSpline = spline.points[indexInSpline];

        float distLeft = (leftSide-point).lenght();
        float distRight = (rightSide-point).lenght();
        float distSpline = (inSpline-point).lenght();
        return distLeft < distSpline || distRight < distSpline;
    }

    void addInterpolatedSuggestion(float yLeft, float yRight) {
        if(spline.isSuggestion) {
            spline.points.clear();
        }
        if (spline.points.size() == 0) {
            Vector3 pointA(lowerLeft.x(), yLeft, lowerLeft.z());
            Vector3 pointB(lowerRigth.x(), yRight, lowerRigth.z());
            spline.addPoint(pointA);
            for (float i = 0.1; i<0.99; i+=0.1) {
                Vector3 add = pointA*i + pointB*(1-i);
                spline.addPoint(add);
            }
            spline.addPoint(pointB);
            spline.isSuggestion = true;
        }
    }

    void setOpposite(SideNode * node);
    void setLeft(SideNode * node);
    void makeSuggestionLines();
    SideNode * copy() {
        SideNode * node = new SideNode(*this);
        return node;
    }
};

#endif // SIDENODE_H
