#include "surface.h"



Surface::Surface(QVector<Vector3> & triangleses,QVector<Vector3> & normals, QVector<Vector3> & outline) {
    for (int i = 0; i<triangleses.size(); i+=3) {
        triangles.append(triangle(triangleses[i],triangleses[i+1],triangleses[i+2], normals[i], normals[i+1], normals[i+2]));
    }

    for (int i = 0; i<outline.size(); ++i) {
        lineVertices.append(vertex(outline[i].x(), outline[i].y(), outline[i].z()));
    }

}
