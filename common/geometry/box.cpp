#include "box.h"
#include <morten3d/Vector3.h>

Box::Box(float width, float heigth, float depth) :
    Shape(), width(width),heigth(heigth),depth(depth)
{
    /*           H ___________ G
                 /|          /|
                / |         / |
               /  |        /  |
              /  E|______ /___|F
             /___/______ /   /
           D|   /       |C  /
            |  /        |  /(depth)
    (height)| /         | /
            |/__________|/
           A    (width)  B
       (0,0,0)

      */
    vertex A (0,0,0,1,1,1,0.5);
    vertex B (width,0,0,1,1,1,0.5);
    vertex C (width,heigth,0,1,1,1,0.5);
    vertex D (0,heigth,0,1,1,1,0.5);

    vertex E (0,0,-depth,1,1,1,0.5);
    vertex F (width,0,-depth,1,1,1,0.5);
    vertex G (width,heigth,-depth,1,1,1,0.5);
    vertex H (0,heigth,-depth,1,1,1,0.5);

    //Front
    vertices.push_back(A);
    vertices.push_back(B);
    vertices.push_back(D);

    vertices.push_back(D);
    vertices.push_back(B);
    vertices.push_back(C);

    //Right
    vertices.push_back(B);
    vertices.push_back(F);
    vertices.push_back(C);

    vertices.push_back(C);
    vertices.push_back(F);
    vertices.push_back(G);

    //Left
    vertices.push_back(E);
    vertices.push_back(A);
    vertices.push_back(H);

    vertices.push_back(H);
    vertices.push_back(A);
    vertices.push_back(D);

    //Bottom
    vertices.push_back(A);
    vertices.push_back(E);
    vertices.push_back(F);

    vertices.push_back(A);
    vertices.push_back(F);
    vertices.push_back(B);

    //Top
    vertices.push_back(D);
    vertices.push_back(G);
    vertices.push_back(H);

    vertices.push_back(D);
    vertices.push_back(C);
    vertices.push_back(G);

    //Back
    vertices.push_back(E);
    vertices.push_back(H);
    vertices.push_back(F);

    vertices.push_back(H);
    vertices.push_back(G);
    vertices.push_back(F);
}

float Box::getDepth() {
    return depth;
}

float Box::getWidth() {
    return width;
}

float Box::getHeight() {
    return heigth;
}




