#ifndef SPLINE_H
#define SPLINE_H

#include <QVector>
#include "morten3d/Vector3.h"
#include <algorithm>
class node;

class Spline
{

public:
    QVector<Vector3> points;
    bool isSuggestion;
    void addPoint(Vector3 point) {
        points.append(point);
        isSuggestion = false;
    }

    Vector3 getPoint(float at);

    void changeLastPoint(Vector3 pos) {
        points[points.size()-1] = pos;
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
        else
            return points[points.size()-1];
    }

    Vector3 getRightPoint() {
        if (isLeftToRight())
            return points[points.size()-1];
        else
            return points[0];
    }

    int findNearestPoint(Vector3 first);

    Spline();
};

#endif // SPLINE_H
