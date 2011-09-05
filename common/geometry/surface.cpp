#include "surface.h"

Surface::Surface(QVector<Spline*>& splines)
{
    Spline& spline = *splines[0];
    for (int i = 1; i< spline.length()/2; ++i) {
        Vector3& p1 = spline.points[i];
        Vector3& p2 = spline.points[i+1];
        Vector3& p3 = spline.points[spline.length() -i];
        Vector3& p4 = spline.points[spline.length() -i-1];
        vertices.append(vertex((float)p1.x(), (float)p1.y(), (float)p1.z(), 0.0f,0.0f,0.0f,1.0f));
        vertices.append(vertex((float)p2.x(), (float)p2.y(), (float)p2.z(), 0.0f,0.0f,0.0f,1.0f));
        vertices.append(vertex((float)p3.x(), (float)p3.y(), (float)p3.z(), 0.0f,0.0f,0.0f,1.0f));
        vertices.append(vertex((float)p2.x(), (float)p2.y(), (float)p2.z(), 0.0f,0.0f,0.0f,1.0f));
        vertices.append(vertex((float)p4.x(), (float)p4.y(), (float)p4.z(), 0.0f,0.0f,0.0f,1.0f));
        vertices.append(vertex((float)p3.x(), (float)p3.y(), (float)p3.z(), 0.0f,0.0f,0.0f,1.0f));
    }
}

QVector<Vector3> Surface::intersectionPoints(Vector3 from,Vector3 direction) {
    return QVector<Vector3>();
}
