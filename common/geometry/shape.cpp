#include "shape.h"
#include <QVector>
#include <float.h>

Shape::Shape() : displayList(-1)
{
}

QVector<Vector3> Shape::intersectionPoints(Vector3 p,Vector3 dir){
    QVector<Vector3> points;

    QVector<vertex> & vertices = getVertices();

    int nearest = -1;
    float distance = FLT_MAX;
    int pointn = 0;
    for(int i=0; i<vertices.size(); i+=3) {
        // hentet fra http://softsurfer.com/Archive/algorithm_0105/algorithm_0105.htm#intersect_RayTriangle%28%29

        Vector3 v0(vertices[i].x,vertices[i].y,vertices[i].z);
        Vector3 v1(vertices[i+1].x,vertices[i+1].y,vertices[i+1].z);
        Vector3 v2(vertices[i+2].x,vertices[i+2].y,vertices[i+2].z);

        Vector3    u, v, n;             // triangle vectors
        Vector3    w0, w;          // ray vectors
        float      r, a, b;             // params to calc ray-plane intersect

        // get triangle edge vectors and plane normal
        u = v1 - v0;
        v = v2 - v0;
        n = u.cross(v);             // cross product
        if (n.x() == 0 && n.y()== 0 && n.z() == 0)            // triangle is degenerate
            continue;  // return -1               // do not deal with this case

        //dir = R.P1 - R.P0;             // ray direction vector
        w0 = p - v0;
        a = -(n * w0);
        b = n * dir;
        if (fabs(b) < 0.01) {     // ray is parallel to triangle plane
            continue;
            /*if (a == 0)                // ray lies in triangle plane
                return 2;
            else return 0;             // ray disjoint from plane*/
        }

        // get intersect point of ray with triangle plane
        r = a / b;
        if (r < 0.0)                   // ray goes away from triangle
            continue; //return 0;                  // => no intersect
        // for a segment, also test if (r > 1.0) => no intersect

        Vector3 I = p + r *dir;           // intersect point of ray and plane

        // is I inside T?
        float    uu, uv, vv, wu, wv, D;
        uu = u*u;
        uv = u*v;
        vv = v*v;
        w = I - v0;
        wu = w*u;
        wv = w*v;
        D = uv * uv - uu * vv;

        // get and test parametric coords
        float s, t;
        s = (uv * wv - vv * wu) / D;
        if (s < 0.0 || s > 1.0)        // I is outside T
            continue; //return 0;
        t = (uv * wu - uu * wv) / D;
        if (t < 0.0 || (s + t) > 1.0)  // I is outside T
            continue; //return 0;

        //return 1;                      // I is in T
        points.append(I);
        if (r < distance) {
            nearest = pointn;
            distance = r;
            pointn++;
        }

    }

    if (pointn > 1) {
        Vector3 & tmp = points[0];
        points[0] = points[nearest];
        points[nearest] = tmp;
    }

    return points;
}
