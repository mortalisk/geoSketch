#include "boxnode.h"

#include <iostream>
#include "GL/glew.h"
#include "surface.h"
#include "float.h"

BoxNode::BoxNode()
{
    width = 10;
    depth = 10;
    heigth = 10;

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
    Vector3 A (0,0,0);
    Vector3 B (width,0,0);
    Vector3 C (width,heigth,0);
    Vector3 D (0,heigth,0);

    Vector3 E (0,0,-depth);
    Vector3 F (width,0,-depth);
    Vector3 G (width,heigth,-depth);
    Vector3 H (0,heigth,-depth);

    QVector<Vector3> vertices;
    QVector<Vector3> lineVertices;

    QVector4D c(1.0, 1.0, 1.0, 0.5);

    //Front
    vertices.push_back(A);
    vertices.push_back(B);
    vertices.push_back(D);

    vertices.push_back(D);
    vertices.push_back(B);
    vertices.push_back(C);

    lineVertices.push_back(A);
    lineVertices.push_back(B);
    lineVertices.push_back(C);
    lineVertices.push_back(D);
    lineVertices.push_back(A);

    front = new Surface(vertices, lineVertices, c);

    vertices.clear();
    lineVertices.clear();
    //Right
    vertices.push_back(B);
    vertices.push_back(F);
    vertices.push_back(C);

    vertices.push_back(C);
    vertices.push_back(F);
    vertices.push_back(G);

    lineVertices.push_back(B);
    lineVertices.push_back(F);
    lineVertices.push_back(G);
    lineVertices.push_back(C);
    lineVertices.push_back(B);

    right = new Surface(vertices, lineVertices, c);

    vertices.clear();
    lineVertices.clear();
    //Left
    vertices.push_back(E);
    vertices.push_back(A);
    vertices.push_back(H);

    vertices.push_back(H);
    vertices.push_back(A);
    vertices.push_back(D);

    lineVertices.push_back(A);
    lineVertices.push_back(E);
    lineVertices.push_back(H);
    lineVertices.push_back(D);
    lineVertices.push_back(A);

    left = new Surface(vertices, lineVertices, c);

    vertices.clear();
    lineVertices.clear();
    //Bottom
    vertices.push_back(A);
    vertices.push_back(E);
    vertices.push_back(F);

    vertices.push_back(A);
    vertices.push_back(F);
    vertices.push_back(B);

    lineVertices.push_back(A);
    lineVertices.push_back(B);
    lineVertices.push_back(F);
    lineVertices.push_back(E);
    lineVertices.push_back(A);

    bottom = new Surface(vertices, lineVertices, c);

    vertices.clear();
    lineVertices.clear();
    //Top
    vertices.push_back(D);
    vertices.push_back(G);
    vertices.push_back(H);

    vertices.push_back(D);
    vertices.push_back(C);
    vertices.push_back(G);

    lineVertices.push_back(D);
    lineVertices.push_back(C);
    lineVertices.push_back(G);
    lineVertices.push_back(H);
    lineVertices.push_back(D);

    top = new Surface(vertices, lineVertices, c);

    vertices.clear();
    lineVertices.clear();
    //Back
    vertices.push_back(E);
    vertices.push_back(H);
    vertices.push_back(F);

    vertices.push_back(H);
    vertices.push_back(G);
    vertices.push_back(F);

    lineVertices.push_back(H);
    lineVertices.push_back(G);
    lineVertices.push_back(F);
    lineVertices.push_back(E);
    lineVertices.push_back(H);

    back = new Surface(vertices, lineVertices, c);

    surfaces.push_back(front);
    surfaces.push_back(back);
    surfaces.push_back(top);
    surfaces.push_back(bottom);
    surfaces.push_back(left);
    surfaces.push_back(right);

}

float BoxNode::getDepth() {
    return depth;
}

float BoxNode::getWidth() {
    return width;
}

float BoxNode::getHeight() {
    return heigth;
}

float BoxNode::intersectionPoint(Vector3 from, Vector3 direction) {
    float dist = FLT_MAX;
    foreach (Surface * s, surfaces) {
        QVector<Vector3> points = s->intersectionPoints(from, direction);
        if (points.size() >0) {
            Vector3& p = points[0];
            float d = (p-from).lenght();
            if (d < dist) dist = d;
        }
    }
    return dist;
}

QVector<Vector3> BoxNode::intersectionPoints(Vector3 from, Vector3 direction) {
    QVector<Vector3> p;
    float d = intersectionPoint(from, direction);
    if (d < FLT_MAX) {
        p.append(from + (direction.normalize()*d));
    }
    return p;
}

void BoxNode::addPoint(Vector3& pos) {
    std::cout << "BoxNode: going to add point" << std::endl;
    if (splines.size() == 0) {
        addSpline();
    }

    Spline* spline = splines[splines.size()-1];


    spline->addPoint(pos);

}

void BoxNode::draw() {
    glDisable(GL_CULL_FACE);
    drawChildren();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    drawSelf();
    glDisable(GL_CULL_FACE);
}

void BoxNode::drawSelf() {
    Node::drawSelf();
    glTranslatef(position.x(), position.y(), position.z());

    foreach(Surface * s, surfaces) {
        s->drawLines();
    }
    foreach(Surface * s, surfaces) {
        s->drawShape();
    }

}
