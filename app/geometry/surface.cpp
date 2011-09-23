#include "surface.h"

Surface::Surface(QVector<Spline*>& splines, QVector4D &color)
{
    Spline& spline = *splines[0];
    for (int i = 1; i< spline.length()/2; ++i) {
        Vector3& p1 = spline.points[i];
        Vector3& p2 = spline.points[i+1];
        Vector3& p3 = spline.points[spline.length() -i];
        Vector3& p4 = spline.points[spline.length() -i-1];
        Vector3 n1 = (p1-p2)*(p1-p3);
        Vector3 n2 = (p2-p3)*(p2-p4);
        triangles.append(triangle(p1, p2,p3, color));
        triangles.append(triangle(p2, p4,p3, color));
    }
}

Surface::Surface(QVector<Vector3> & triangleses, QVector<Vector3> & outline, QVector4D & color) {
    for (int i = 0; i<triangleses.size(); i+=3) {
        triangles.append(triangle(triangleses[i],triangleses[i+1],triangleses[i+2], color));
    }

    for (int i = 0; i<outline.size(); ++i) {
        lineVertices.append(vertex(outline[i].x(), outline[i].y(), outline[i].z()));
    }

}
