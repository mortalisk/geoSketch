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

    void redrawPoints() {
        spline.clear();
        foreach(QVector2D p, uvSpline.getPoints()) {
            spline.addPoint(getPointFromUv(p));
        }
    }

    void projectPoints(Vector3 diff,const QVector<QVector2D>& points) {

        for (int i = points.size()-1; i>=0; --i) {
            uvSpline.addPoint(QVector2D(1.0-points[i].x(),points[i].y()));
        }
    }
    bool includeShapeInExport() {return true;}

    void addSubclassActions(QToolBar *) {};

    bool isPointNearerSide(Vector3 &point, int indexInSpline) {
        Vector3 leftSide(lowerLeft.x(), point.y(), lowerLeft.z());
        Vector3 rightSide(lowerRigth.x(), point.y(), lowerRigth.z());
        const Vector3& inSpline = spline.getPoints()[indexInSpline];

        float distLeft = (leftSide-point).lenght();
        float distRight = (rightSide-point).lenght();
        float distSpline = (inSpline-point).lenght();
        return distLeft < distSpline || distRight < distSpline;
    }

    Spline getPoints(Spline2d & s) {
        Spline r;
        foreach(QVector2D p, s.getPoints()) {
            Vector3 toAdd = getPointFromUv(p);
            r.addPoint(toAdd);
        }
        return r;
    }

    Vector3 getPointFromUv(QVector2D point) {
        Vector3 a = (lowerLeft*(1.0-point.x()) + lowerRigth * point.x());
        Vector3 b = (lowerLeft*(1.0-point.y()) + upperLeft * point.y());
        return a + b - lowerLeft;
    }

    void addInterpolatedSuggestion(float yLeft, float yRight) {
        ensureLeftToRigth();
        if(uvSpline.isSuggestion) {
            uvSpline.clear();

            QVector2D pointA(0, yLeft);
            QVector2D pointB(1, yRight);
            for (float i = 0.0; i<1.01; i+=0.05) {
                QVector2D add = interpolate(pointA, pointB, i);
                uvSpline.addPoint(add);
            }
            uvSpline.isSuggestion = true;
        }else if (uvSpline.getPoints().size() > 0){
            const QVector<QVector2D> & points = uvSpline.getPoints();
            QVector2D first = points[0];
            QVector2D last = points[points.size()-1];
            float length = 0.0;
            for (int i = 1; i< points.size();++i) {
                length += (points[i-1]-points[i]).length();
            }
            float along = 0.0;
            QVector2D previous = first;
            for (int i = 0; i <points.size(); ++i) {
                along += (previous-points[i]).length();
                previous = points[i];
                float w = along/length;
                float targetY = yLeft*(1.0-w) + yRight*w;
                float lineY = first.y()*(1.0-w)+last.y()*w;
                float diff = points[i].y() - lineY;
                float newY = targetY + diff;
                uvSpline.setPoint(i, QVector2D(points[i].x(), newY));
            }
        }
    }

    QVector2D interpolate(QVector2D pointA, QVector2D pointB, float t) {
        return (pointA*t) + (pointB*(1.0-t));
    }

    void ensureLeftToRigth() {
        if (!uvSpline.isLeftToRight()) {
            uvSpline.reverse();
        }
    }

    void setOpposite(SideNode * node);
    void setLeft(SideNode * node);
    void makeSuggestionLines();

    void addSubclassJson(QVariantMap &map);
    void fromJsonSubclass(QVariantMap map);

};

#endif // SIDENODE_H
