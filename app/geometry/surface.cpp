#include "surface.h"



Surface::Surface(QVector<vertex> & triangleses, QVector<Vector3> & outline,bool strip) {
    triangles = triangleses;
    this->strip = strip;
}
