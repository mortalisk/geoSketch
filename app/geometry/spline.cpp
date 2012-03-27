#include "spline.h"
#include "float.h"

Spline::Spline() : isSuggestion(true)
{
}

int Spline::findNearestPoint(Vector3 searchPoint) {
        int nearest = -1;
        float distance = FLT_MAX;
        for (int i = 0; i < points.size(); ++i) {
                Vector3 & point = points[i];
                float distanceThisFirst = (point - searchPoint).lenght();
                if (distanceThisFirst < distance) {
                        nearest = i;
                        distance = distanceThisFirst;
                }
        }

        return nearest;
}

void Spline::smooth() {
    if (this->points.size()<1) return;
    QVector<Vector3> newPoints;
    for (float p = 0; p <1.001;p+=0.01) {
        QVector<Vector3> points = this->points;
        for (int i = 1; i< points.size(); ++i) {
            for (int j = 0;j <points.size()-i;++j) {
                points[j] = points[j]*(1-p) + points[j+1]*p;
            }
        }
        newPoints.push_back(points[0]);
    }
    this->points = newPoints;
}

Vector3 Spline::getPoint(float at) {
    if (at > 1.0) {
        at = 1.0;
    }else if (at < 0.0) {
        at = 0.0;
    }

    float length = 0.0;
    for (int i = 0; i < points.size()-1; i++) {
        length += (points[i]-points[i+1]).lenght();
    }
    float pos = 0.0;
    float target = length*at;
    Vector3 r;
    for (int i = 0; i < points.size()-1; i++) {
        pos += (points[i]-points[i+1]).lenght();
        if(pos >= target) {
            float overshoot = pos-target;
            float lengthBetween = (points[i]-points[i+1]).lenght();
            float a = overshoot/lengthBetween;
            r = points[i+1]*(1-a) + (points[i]*(a));
            return r;
        }
    }
    return r;

}
