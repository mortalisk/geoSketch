#include "deposit.h"
#include "surface.h"

void Deposit::prepareForDrawing() {
    if (shape == NULL) {
        float heigth = 0.1;
        float radius = volume/heigth/2;
        QVector2D center2d = flowFrom + direction.normalized()*radius;
        Vector3 center3d = surfaceNode->getPointFromUv(center2d);
        center3d = Vector3(center3d.x(), center3d.y()+heigth*10, center3d.z());

        QVector<vertex> triangleStrip;
        float deg60 = M_PI/6;
        triangleStrip.push_back(vertex(center3d, Vector3(0,1,0)));
        for (float t = 0.0; t <= M_PI; t += deg60 ) {
            float x = sin(deg60)*radius;
            float y = cos(deg60)*radius;
            QVector2D p(x,y);
            Vector3 point = surfaceNode->getPointFromUv(p);
            triangleStrip.push_back(vertex(point, Vector3(0,1,0)));
        }
        QVector<Vector3> noOutline;
        shape = new Surface(triangleStrip, noOutline, true);
    }
}
