#include "spline.h"

Spline::Spline()
{
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
