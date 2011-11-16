#include "spline.h"
#include "float.h"

Spline::Spline()
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

Vector3 Spline::getPoint(float at) {

    float length = 0.0;
    for (int i = 0; i < points.size()-1; i++) {
        length += (points[i]-points[i+1]).lenght();
    }
    float pos = 0.0;
    Vector3 r;
    for (int i = 0; i < points.size()-1; i++) {
        pos += (points[i]-points[i+1]).lenght();
        if(pos >= at*length) {
            float backtrack = pos/length - at;

            r = points[i] + (points[i+1] - (points[i]*(1-backtrack)));


        }
    }
    return r;

}
