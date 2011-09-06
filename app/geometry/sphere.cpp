#include "sphere.h"
#include <math.h>
#include "arrays.h"

Sphere::Sphere(float radius, float red, float g, float b) {
    int antallStykker =16;
    int antallDisker = 8;

    vertex top (0,radius,0,red,g,b,0);
    vertex punkter[antallDisker][antallStykker];
    vertex bunn (0,-radius,0,red,g,b,0);

    for (int d = 0; d<antallDisker; d++) {
        float phi = d/(float)antallDisker*M_PI;
        float r = sin( phi ) * radius;
        float y = cos( phi ) * radius;
        for (int s = 0; s < antallStykker; s++) {
            float theta = s/(float)antallStykker * M_PI*2;
            float x = cos (theta) * r;
            float z = sin (theta) * r;
            vertex v (x,y,z, red,g,b,1.0f);
            punkter[d][s] = v;
        }
    }


    // overste
    for(int s = 0; s<antallStykker; s++) {
        this->vertices.push_back(punkter[1][s]);
        this->vertices.push_back(top);
        this->vertices.push_back(punkter[1][(s+1)%antallStykker]);
    }
    // diskene
    for (int d = 2; d<antallDisker; d++) {
        for(int s = 0; s<antallStykker; s++) {
            this->vertices.push_back(punkter[d][s]);
            this->vertices.push_back(punkter[d-1][(s+1)%antallStykker]);
            this->vertices.push_back(punkter[d-1][s]);
            this->vertices.push_back(punkter[d][s]);
            this->vertices.push_back(punkter[d][(s+1)%antallStykker]);
            this->vertices.push_back(punkter[d-1][(s+1)%antallStykker]);
        }
    }
    // nederste
    for(int s = 0; s<antallStykker; s++) {
        this->vertices.push_back(punkter[antallDisker-1][s]);
        this->vertices.push_back(bunn);
        this->vertices.push_back(punkter[antallDisker-1][(s+1)%antallStykker]);
    }

}


