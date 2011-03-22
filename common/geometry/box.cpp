#include "box.h"

Box::Box(float width, float heigth, float depth, QObject *parent) :
    QObject(parent), width(width),heigth(heigth),depth(depth)
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
    QVector3D A(0,0,0);
    QVector3D B(width,0,0);
    QVector3D C(width,heigth,0);
    QVector3D D(0,heigth,0);

    QVector3D E(0,0,-depth);
    QVector3D F(width,0,-depth);
    QVector3D G(width,heigth,-depth);
    QVector3D H(0,heigth,-depth);

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
    vertices.push_back(F);
    vertices.push_back(E);

    vertices.push_back(A);
    vertices.push_back(B);
    vertices.push_back(F);

    //Top
    vertices.push_back(D);
    vertices.push_back(G);
    vertices.push_back(H);

    vertices.push_back(D);
    vertices.push_back(C);
    vertices.push_back(G);

    //Back
    vertices.push_back(E);
    vertices.push_back(F);
    vertices.push_back(H);

    vertices.push_back(H);
    vertices.push_back(F);
    vertices.push_back(G);
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

QVector<QVector3D> & Box::getVertices(){
    return vertices;
}
