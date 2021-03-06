
#include <GL/glew.h>
#include "shape.h"
#include <QVector>
#include <float.h>

Shape::Shape() : displayList(-1)
{
}

Shape::~Shape() {
}

void Shape::drawLines(bool stipple, float red, float green, float blue, float alpha) {
    glLineWidth(2.0);
    float c[] = {red,green,blue,alpha};
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,c);
    glColor4fv(c);
    if (stipple) {
        glLineStipple(1, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
    }
    glEnableClientState(GL_VERTEX_ARRAY);    
    if (lineVertices.size() > 0) {
        glVertexPointer(3,GL_FLOAT,sizeof(vertex),&lineVertices[0]);
        glDrawArrays(GL_LINE_STRIP,0,lineVertices.size());
    }
    if (stipple) {
        glDisable(GL_LINE_STIPPLE);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Shape::drawShape(QVector4D ambient, QVector4D diffuse) {
    float c[] = {diffuse.x(), diffuse.y(), diffuse.z(), diffuse.w() };
    glMaterialfv(GL_FRONT,GL_DIFFUSE,c);
    float spec[] = {diffuse.x()+0.5, diffuse.y()+0.5, diffuse.z()+0.5, diffuse.w()};
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f );
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    float a[] = {diffuse.x()/3, diffuse.y()/3, diffuse.z()/3, ambient.w() };
    glMaterialfv(GL_FRONT,GL_AMBIENT,a);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnableClientState(GL_NORMAL_ARRAY);
    if (triangles.size() > 0) {
        glVertexPointer(3,GL_FLOAT,sizeof(vertex),&triangles[0]);
        glNormalPointer(GL_FLOAT,sizeof(vertex),&triangles[0].n1);
        if(!strip)
            glDrawArrays(GL_TRIANGLES,0,triangles.size());
        else
            glDrawArrays(GL_TRIANGLE_STRIP,0,triangles.size());
    }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

}

QVector<Vector3> Shape::intersectionPoints(Vector3 p,Vector3 dir, float& rs, float& rt){
    QVector<Vector3> points;

    QVector<vertex> & triangles = this->triangles;

    int nearest = -1;
    float distance = FLT_MAX;
    int pointn = 0;
    int inc = strip?1:3;
    for(int i=0; i<triangles.size()-2; i+=inc) {
        // hentet fra http://softsurfer.com/Archive/algorithm_0105/algorithm_0105.htm#intersect_RayTriangle%28%29

        Vector3 v0(triangles[i].x,triangles[i].y,triangles[i].z);
        Vector3 v1(triangles[i+1].x,triangles[i+1].y,triangles[i+1].z);
        Vector3 v2(triangles[i+2].x,triangles[i+2].y,triangles[i+2].z);

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
            float s1 = triangles[i].s;
            float s2 = triangles[i+1].s;
            float s3 = triangles[i+2].s;
            float t1 = triangles[i].t;
            float t2 = triangles[i+1].t;
            float t3 = triangles[i+2].t;
            rs = s1 + (s2-s1)*s + (s3-s1)*t;
            rt = t1 + (t2-t1)*s + (t3-t1)*t;
        }

    }

    if (pointn > 1) {
        Vector3 & tmp = points[0];
        points[0] = points[nearest];
        points[nearest] = tmp;
    }

    return points;
}
