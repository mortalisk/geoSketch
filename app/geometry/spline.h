#ifndef SPLINE_H
#define SPLINE_H

#include <QVector>
#include "morten3d/Vector3.h"
#include <algorithm>
#include "shape.h"
#include <QMap>
class node;

class Spline : public Shape
{

    QVector<Vector3> points;
public:
    bool isSuggestion;
    QMap<float, Vector3> pointsCache;
    void addPoint(Vector3 point) {
        points.append(point);
        isSuggestion = false;
        pointsCache.clear();
    }

    void addPointFront(Vector3 point) {
        points.push_front(point);
        isSuggestion = false;
        pointsCache.clear();
    }

    void addAll(const QVector<Vector3> & a) {
        points += a;
    }

    const QVector<Vector3>& getPoints() {
        return points;
    }

    void setPoint(int i, Vector3 value) {
        points[i] = value;
        pointsCache.clear();
    }

    void reverse() {
        std::reverse(points.begin(),points.end());
        pointsCache.clear();
    }

    void clear() {
        points.clear();
        pointsCache.clear();
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

    Spline();
};

#endif // SPLINE_H
