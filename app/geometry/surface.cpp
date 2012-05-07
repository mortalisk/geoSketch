#include "surface.h"



Surface::Surface(QVector<vertex> & triangleses, QVector<Vector3> & outline,bool strip) {
    triangles = triangleses;

    for (int i = 0; i<outline.size(); ++i) {
        lineVertices.append(vertex(outline[i].x(), outline[i].y(), outline[i].z()));
    }

    this->strip = strip;
}
