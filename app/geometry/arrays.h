#ifndef ARRAYS_H
#define ARRAYS_H
#include "Vector3.h"
#include <QVector4D>

struct vertex {

    vertex(float x = 0, float y = 0, float z = 0, float r = 0, float g = 0, float b = 0, float a = 0, float n1 = 0, float n2 = 0, float n3 = 0) : x(x), y(y), z(z), r(r), g(g), b(b), a(a), n1(n1), n2(n2), n3(n3) {

    }

    vertex(Vector3 & v, QVector4D & c, Vector3 & n) {
        x = v.x();
        y = v.y();
        z = v.z();
        r = c.x();
        g = c.y();
        b = c.z();
        a = c.w();
        n1 = n.x();
        n2 = n.y();
        n3 = n.z();
    }

    float x,y,z;
    float r,g,b,a;
    float n1, n2, n3;
};

struct triangle {
    vertex p1, p2, p3;

    triangle() {

    }

    /** expects points in CCW order to make normal */
    triangle(Vector3& v1, Vector3& v2, Vector3& v3, QVector4D & c) {
        Vector3 n = ((v2-v1).cross(v3-v1)).normalize();
        p1 = vertex(v1.x(), v1.y(), v1.z(),c.x(), c.y(), c.z(), c.w(), n.x(), n.y(), n.z());
        p2 = vertex(v2.x(), v2.y(), v2.z(),c.x(), c.y(), c.z(), c.w(), n.x(), n.y(), n.z());
        p3 = vertex(v3.x(), v3.y(), v3.z(),c.x(), c.y(), c.z(), c.w(), n.x(), n.y(), n.z());
    }
};


#endif // ARRAYS_H
