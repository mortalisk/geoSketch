#ifndef SPLINE_H
#define SPLINE_H

#include <QVector>
#include "morten3d/Vector3.h"
class node;

class Spline
{

public:
    QVector<Vector3> points;
    void addPoint(Vector3& point) {
        points.append(point);
    }

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

    Vector3 katmullRom(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3) {
            float t2 = t * t;
            float t3 = t2 * t;
            Vector3 pos = ( ( p1 * 2.0f) +
                            ( -p0 + p2 ) * t +
                            ( p0 * 2.0f - p1 * 5.0f + p2 * 4 - p3 ) * t2 +
                            ( -p0 + p1 * 3.0f - p2 * 3.0f + p3 ) * t3 ) * 0.5f;
            return pos;
    }

    Spline();
};

#endif // SPLINE_H
