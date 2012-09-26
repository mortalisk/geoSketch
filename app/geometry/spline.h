#ifndef SPLINE_H
#define SPLINE_H

#include <QVector>
#include "morten3d/Vector3.h"
#include <algorithm>
#include "shape.h"
#include <QMap>
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

    const QVector<Vector3>& getPoints() {
        return points;
    }

    void elevate(float y) {
        for(int i=0; i<points.size(); i++) {
            points[i] = Vector3(points[i].x(), points[i].y()+y, points[i].z());
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
