#include "spline2DE.h"
#include "float.h"

Spline2d::Spline2d() : isSuggestion(true)
{
}

int Spline2d::findNearestPoint(QVector2D searchPoint) {
        int nearest = -1;
        float distance = FLT_MAX;
        for (int i = 0; i < points.size(); ++i) {
                QVector2D & point = points[i];
                float distanceThisFirst = (point - searchPoint).length();
                if (distanceThisFirst < distance) {
                        nearest = i;
                        distance = distanceThisFirst;
                }
        }

        return nearest;
}

void Spline2d::smooth() {
    if (this->points.size()<1) return;
    QVector<QVector2D> newPoints;
    for (float p = 0; p <1.001;p+=0.01) {
        QVector<QVector2D> points = this->points;
        for (int i = 1; i< points.size(); ++i) {
            for (int j = 0;j <points.size()-i;++j) {
                points[j] = points[j]*(1-p) + points[j+1]*p;
            }
        }
        newPoints.push_back(points[0]);
    }
    this->points = newPoints;
    pointsCache.clear();
}

QVector2D Spline2d::getPoint(float at) {
    if (at > 1.0) {
        at = 1.0;
    }else if (at < 0.0) {
        at = 0.0;
    }
    if (pointsCache.contains(at)) {
        return pointsCache[at];
    }

    float length = 0.0;
    for (int i = 0; i < points.size()-1; i++) {
        length += (points[i]-points[i+1]).length();
    }
    float pos = 0.0;
    float target = length*at;
    QVector2D r;
    for (int i = 0; i < points.size()-1; i++) {
        pos += (points[i]-points[i+1]).length();
        if(pos >= target) {
            float overshoot = pos-target;
            float lengthBetween = (points[i]-points[i+1]).length();
            float a = overshoot/lengthBetween;
            r = points[i+1]*(1-a) + (points[i]*(a));
            return r;
        }
    }
    pointsCache.insert(at,r);
    return r;

}
