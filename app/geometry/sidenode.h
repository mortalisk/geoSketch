#ifndef SIDENODE_H
#define SIDENODE_H
#include "shape.h"
#include "basenode.h"

class SideNode : public BaseNode
{
    Q_OBJECT
public:
    SideNode * opposite;
    SideNode * left;
    SideNode * right;
    Vector3 lowerLeft;
    Vector3 lowerRigth;
    Vector3 upperRigth;
    Vector3 upperLeft;
    SideNode(Vector3 lowerLeft,Vector3 lowerRigth, Vector3 upperRigth,Vector3 upperLeft);
    SideNode(SideNode& o) :BaseNode(o), lowerLeft(o.lowerLeft), lowerRigth(o.lowerRigth), upperRigth(o.upperRigth), upperLeft(o.upperLeft) {
        init();
    }

    SideNode() {}

    void init();

    BaseNode * copy() {
        return new SideNode(*this);
    }

    QString getTypeId() {
        return QString("SideNode");
    }

    void projectPoints(Vector3 diff,const QVector<Vector3>& points) {

        foreach(const Vector3& point, points) {
                spline.addPoint(point+diff);
        }
    }

    bool isPointNearerSide(Vector3 &point, int indexInSpline) {
        Vector3 leftSide(lowerLeft.x(), point.y(), lowerLeft.z());
        Vector3 rightSide(lowerRigth.x(), point.y(), lowerRigth.z());
        const Vector3& inSpline = spline.getPoints()[indexInSpline];

        float distLeft = (leftSide-point).lenght();
        float distRight = (rightSide-point).lenght();
        float distSpline = (inSpline-point).lenght();
        return distLeft < distSpline || distRight < distSpline;
    }

    void addInterpolatedSuggestion(float yLeft, float yRight) {
        ensureLeftToRigth();
        if(spline.isSuggestion) {
            spline.clear();

            Vector3 pointA(lowerLeft.x(), yLeft, lowerLeft.z());
            Vector3 pointB(lowerRigth.x(), yRight, lowerRigth.z());
            for (float i = 0.0; i<1.01; i+=0.05) {
                Vector3 add = interpolate(pointA, pointB, i);
                spline.addPoint(add);
            }
            spline.isSuggestion = true;
        }else {
            const QVector<Vector3> & points = spline.getPoints();
            const Vector3& first = points[0];
            Vector3 last = points[points.size()-1];
            float length = 0.0;
            for (int i = 1; i< points.size();++i) {
                length += (points[i-1]-points[i]).lenght();
            }
            float along = 0.0;
            Vector3 previous = first;
            for (int i = 0; i <points.size(); ++i) {
                along += (previous-points[i]).lenght();
                previous = points[i];
                float w = along/length;
                float targetY = yLeft*(1.0-w) + yRight*w;
                float lineY = first.y()*(1.0-w)+last.y()*w;
                float diff = points[i].y() - lineY;
                float newY = targetY + diff;
                spline.setPoint(i, Vector3(points[i].x(), newY, points[i].z()));
            }
        }
    }

    Vector3 interpolate(Vector3 pointA, Vector3 pointB, float t) {
        return (pointA*t) + (pointB*(1.0-t));
    }

    void ensureLeftToRigth() {
        if (!spline.isLeftToRight()) {
            spline.reverse();
        }
    }

    void setOpposite(SideNode * node);
    void setLeft(SideNode * node);
    void makeSuggestionLines();

    void addSubclassJson(QVariantMap &map);
    void fromJsonSubclass(QVariantMap map);

};

#endif // SIDENODE_H
