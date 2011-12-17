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

    void projectPoints(Vector3 diff,QVector<Vector3>& points) {

        foreach(const Vector3& point, points) {
                spline.addPoint(point+diff);
        }
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
            for (float i = 0.0; i<1.01; i+=0.05) {
                Vector3 add = interpolate(pointA, pointB, i);
                spline.addPoint(add);
            }
            spline.isSuggestion = true;
        }
    }

    Vector3 interpolate(Vector3 pointA, Vector3 pointB, float t) {
        return (pointA*t) + (pointB*(1.0-t));
    }

    void ensureLeftToRigth() {
        if (!spline.isLeftToRight()) {
            std::reverse(spline.points.begin(),spline.points.end());
        }
    }

    void setOpposite(SideNode * node);
    void setLeft(SideNode * node);
    void makeSuggestionLines();

};

#endif // SIDENODE_H
