#ifndef ARRAYS_H
#define ARRAYS_H
#include "Vector3.h"
#include <QVector4D>

struct vertex {

    vertex(float x = 0, float y = 0, float z = 0, float n1 = 0, float n2 = 0, float n3 = 0, float s=0, float t=0) : x(x), y(y), z(z), n1(n1), n2(n2), n3(n3), s(s), t(t) {

    }

    vertex(const Vector3 & v, const Vector3 & n, float s=-1, float t=-1) : x(v.x()) , y(v.y()), z(v.z()), n1(n.x()), n2(n.y()), n3(n.z()), s(s), t(t)  {

    }

    float x,y,z;
    float n1, n2, n3;
    float s, t;
};

struct triangle {
    vertex p1, p2, p3;

    triangle() {

    }

    /** expects points in CCW order to make normal */
    triangle(Vector3& v1, Vector3& v2, Vector3& v3) {
        Vector3 n = ((v2-v1).cross(v3-v1)).normalize();
        p1 = vertex(v1.x(), v1.y(), v1.z(), n.x(), n.y(), n.z());
        p2 = vertex(v2.x(), v2.y(), v2.z(), n.x(), n.y(), n.z());
        p3 = vertex(v3.x(), v3.y(), v3.z(), n.x(), n.y(), n.z());
    }

    triangle(Vector3& v1, Vector3& v2, Vector3& v3, Vector3& n1,  Vector3& n2,  Vector3& n3) {
        p1 = vertex(v1.x(), v1.y(), v1.z(), n1.x(), n1.y(), n1.z());
        p2 = vertex(v2.x(), v2.y(), v2.z(), n2.x(), n2.y(), n2.z());
        p3 = vertex(v3.x(), v3.y(), v3.z(), n3.x(), n3.y(), n3.z());
    }
};


#endif // ARRAYS_H
