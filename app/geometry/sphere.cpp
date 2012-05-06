#define _USE_MATH_DEFINES
#include "sphere.h"
#include <math.h>
#include "arrays.h"

Sphere::Sphere(float radius) {
    const int antallStykker =16;
    const int antallDisker = 8;

    Vector3 top (0,radius,0);
    Vector3 punkter[antallDisker][antallStykker];
    Vector3 bunn (0,-radius,0);
    QVector4D c(1.0, 0, 0, 1.0);

    for (int d = 0; d<antallDisker; d++) {
        float phi = d/(float)antallDisker*M_PI;
        float r = sin( phi ) * radius;
        float y = cos( phi ) * radius;
        for (int s = 0; s < antallStykker; s++) {
            float theta = s/(float)antallStykker * M_PI*2;
            float x = cos (theta) * r;
            float z = sin (theta) * r;
            Vector3 v (x,y,z);
            punkter[d][s] = v;
        }
    }


    // overste
    for(int s = 0; s<antallStykker; s++) {
        this->triangles.push_back(vertex(punkter[1][s],punkter[1][s]));
        this->triangles.push_back(vertex(top,top));
        this->triangles.push_back(vertex(punkter[1][(s+1)%antallStykker],punkter[1][(s+1)%antallStykker]));
    }
    // diskene
    for (int d = 2; d<antallDisker; d++) {
        for(int s = 0; s<antallStykker; s++) {
            this->triangles.push_back(vertex(punkter[d-1][s],punkter[d-1][s]));
            this->triangles.push_back(vertex(punkter[d-1][(s+1)%antallStykker],punkter[d-1][(s+1)%antallStykker]));
            this->triangles.push_back(vertex(punkter[d][s],punkter[d][s]));

            this->triangles.push_back(vertex(punkter[d-1][(s+1)%antallStykker],punkter[d-1][(s+1)%antallStykker]));
            this->triangles.push_back(vertex(punkter[d][(s+1)%antallStykker],punkter[d][(s+1)%antallStykker]));
            this->triangles.push_back(vertex(punkter[d][s],punkter[d][s]));
        }
    }
    // nederste
    for(int s = 0; s<antallStykker; s++) {
        this->triangles.push_back(vertex(punkter[antallDisker-1][(s+1)%antallStykker],punkter[antallDisker-1][(s+1)%antallStykker]));
        this->triangles.push_back(vertex(bunn,bunn));
        this->triangles.push_back(vertex(punkter[antallDisker-1][s],punkter[antallDisker-1][s]));
    }

}


