#ifndef SPLINE_H
#define SPLINE_H

#include <QVector>
#include "morten3d/Vector3.h"
#include <algorithm>
#include "shape.h"
#include <QMap>
#include "util.h"
class node;

class Spline
{

    QVector<Vector3> points;

public:
     int drawFrom, drawUntil;
    bool isSuggestion;
    QMap<float, Vector3> pointsCache;
    void addPoint(Vector3 point) {
        points.append(point);
        isSuggestion = false;
        pointsCache.clear();
        resetDrawPoints();
    }

    void addPointFront(Vector3 point) {
        points.push_front(point);
        isSuggestion = false;
        pointsCache.clear();
        resetDrawPoints();
    }

    void resetDrawPoints() {
        drawFrom = 0;
        drawUntil = points.size();
    }

    void addAll(const QVector<Vector3> & a) {
        points += a;
        resetDrawPoints();
    }

    void updateForBelowNode(Spline & newLayer) {
        if (!isLeftToRight()) reverse();
        if (!newLayer.isLeftToRight()) newLayer.reverse();
        Vector3& splinep1 = points[0];
        Vector3& splinep2 = points[points.size()-1];
        Vector3& pfromnew = newLayer.points[0];
        QVector<Vector3> newPoints;
        Axis axis = X;
        bool similarZ = similar(splinep1.z(),pfromnew.z(),splinep2.z());
        if (similarZ) {
            axis = Z;
        }
        int intersectionIndex = 0;
        int intersectionIndexTop = 0;
        for (int i = 0; i< points.size()-1; i++) {
            for (int j = 0;j <newLayer.points.size()-1; j++) {
                Vector3& a = points[i];
                Vector3& b = points[i+1];
                Vector3& c = newLayer.points[j];
                Vector3& d = newLayer.points[j+1];

                double x,y;
                if (axis == X && lineSegmentIntersection(a.z(),a.y(), b.z(), b.y(), c.z(), c.y(), d.z(), d.y(), &x, &y)) {
                    Vector3 intersect(a.x(),y,x);

                    if (points[i].y() > newLayer.points[j].y()) {
                        for (int k = intersectionIndex; k <= i; k++) {
                            newPoints.push_back(points[k]);
                        }
                    }else {
                        for (int k = intersectionIndexTop; k <= j; k++) {
                            newPoints.push_back(newLayer.points[k]);
                        }
                    }
                    newPoints.push_back(intersect);


                    intersectionIndex = i+1;
                    intersectionIndexTop = j+1;

                }else if (axis == Z && lineSegmentIntersection(a.x(),a.y(), b.x(), b.y(), c.x(), c.y(), d.x(), d.y(), &x, &y)) {
                    Vector3 intersect(x,y,a.z());

                    if (points[i].y() > newLayer.points[j].y()) {
                        for (int k = intersectionIndex; k <= i; k++) {
                            newPoints.push_back(points[k]);
                        }
                    }else {
                        for (int k = intersectionIndexTop; k <= j; k++) {
                            newPoints.push_back(newLayer.points[k]);
                        }
                    }
                    newPoints.push_back(intersect);


                    intersectionIndex = i+1;
                    intersectionIndexTop = j+1;
                }

            }
        }

        if(points[points.size()-1].y() > newLayer.points[newLayer.points.size()-1].y()) {
            for (int i = intersectionIndex; i< points.size(); i++) {
                newPoints.push_back(points[i]);
            }
        } else {
            for (int i = intersectionIndexTop; i< newLayer.points.size(); i++) {
                newPoints.push_back(newLayer.points[i]);
            }
        }

        points = newPoints;
    }

    const QVector<Vector3>& getPoints() {
        return points;
    }

    void elevateTo(float y) {
        for(int i=0; i<points.size(); i++) {
            points[i] = Vector3(points[i].x(), y, points[i].z());
        }
    }

    void setPoint(int i, Vector3 value) {
        points[i] = value;
        pointsCache.clear();
        resetDrawPoints();
    }

    void reverse() {
        std::reverse(points.begin(),points.end());
        pointsCache.clear();
    }

    void clear() {
        points.clear();
        pointsCache.clear();
        resetDrawPoints();
    }

    void smooth();
    Vector3 getPoint(float at);

    void changeLastPoint(Vector3 pos) {
        points[points.size()-1] = pos;
        pointsCache.clear();
    }

    Vector3 lastPoint() {
        if (points.size() > 1) {
        return points[points.size()-2];
        }else {
            Vector3 r(-10000,-10000,-10000);
            return r;
        }
    }

    int length() {
        return points.size();
    }

    bool isLeftToRight() {
        if (points.size() == 0) return false;
        return points[0].isLeftOf(points[points.size()-1]);
    }

    Vector3 getLeftPoint() {
        if (isLeftToRight())
            return points[0];
        else if(points.size()>0)
            return points[points.size()-1];
        else {
            std::cout << "getLeftPoint called, but there are no points" << std::endl;
            return Vector3();
        }
    }

    Vector3 getRightPoint() {
        if (isLeftToRight())
            return points[points.size()-1];
        else if (points.size() > 0)
            return points[0];
        else {
            std::cout << "getLeftPoint called, but there are no points" << std::endl;
            return Vector3();
        }
    }

    int findNearestPoint(Vector3 first);

    QVariantMap toJson() {
        QVariantMap map;
        QVariantList pointsList;
        foreach(Vector3 point, points) {
            pointsList.append(point.toJson());
        }
        map["points"] = pointsList;
        return map;
    }

    void fromJson(QVariantMap map) {
        points.clear();
        QVariantList list = map["points"].toList();
        foreach(QVariant v, list) {
            points.append(Vector3::newfromJson(v.toMap()));
        }
    }

    Spline();
};

#endif // SPLINE_H
