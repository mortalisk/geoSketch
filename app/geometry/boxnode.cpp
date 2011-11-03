#include "boxnode.h"

#include <iostream>
#include <algorithm>
#include "GL/glew.h"
#include "surface.h"
#include "float.h"

BoxNode::BoxNode()
{
    activeSurface = NULL;
    width = 10;
    depth = 10;
    heigth = 10;
    topF = heigth/2;
    bottomF = -topF;
    rightF = width/2;
    leftF = -rightF;
    farF = depth/2;
    nearF = -farF;

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
    Vector3 A (leftF,bottomF,nearF);
    Vector3 B (rightF,bottomF,nearF);
    Vector3 C (rightF,topF,nearF);
    Vector3 D (leftF,topF,nearF);

    Vector3 E (leftF,bottomF,farF);
    Vector3 F (rightF,bottomF,farF);
    Vector3 G (rightF,topF,farF);
    Vector3 H (leftF,topF,farF);

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

    Surface * front = new Surface(vertices, lineVertices, c);

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

    Surface * right = new Surface(vertices, lineVertices, c);

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

    Surface * left = new Surface(vertices, lineVertices, c);

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

    Surface * bottom = new Surface(vertices, lineVertices, c);

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

    Surface * top = new Surface(vertices, lineVertices, c);

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

    Surface * back = new Surface(vertices, lineVertices, c);

    frontNode = new SideNode(front);
    backNode = new SideNode(back);
    topNode = new SideNode(top);
    bottomNode = new SideNode(bottom);
    leftNode = new SideNode(left);
    rightNode = new SideNode(right);

    frontNode->setOpposite(backNode);
    leftNode->setOpposite(rightNode);
    topNode->setOpposite(bottomNode);

    frontNode->setLeft(leftNode);
    leftNode->setLeft(backNode);
    backNode->setLeft(rightNode);
    rightNode->setLeft(frontNode);

    surfaces.push_back(frontNode);
    surfaces.push_back(backNode);
    surfaces.push_back(topNode);
    surfaces.push_back(bottomNode);
    surfaces.push_back(leftNode);
    surfaces.push_back(rightNode);

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
    foreach (Node * s, surfaces) {
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

void BoxNode::addPoint(Vector3 from, Vector3 direction) {

    float candidateDistance = FLT_MAX;
    Vector3 candidatePoint;
    SideNode * candidate = NULL;

    foreach (SideNode * s, surfaces) {
        QVector<Vector3> points = s->intersectionPoints(from, direction);
        if (points.size() >0 ) {
                float dist = (points[0]-from).lenght();
                if ((activeSurface == NULL || s == activeSurface) && dist < candidateDistance) {
                    candidateDistance = dist;
                    candidatePoint = points[0];
                    candidate=s;
               }

        }
    }

    if (candidate) {
        candidate->spline.addPoint(candidatePoint);

        activeSurface = candidate;
    }
}

void BoxNode::stopDrawing() {
    if (activeSurface) {
        QVector<Vector3>& pts = activeSurface->spline.points;
        int first = 0, last = pts.size()-1;
        if (activeSurface == frontNode) {
            if (pts[first].x() > pts[last].x()) {
                std::reverse(pts.begin(),pts.end());
            }
            pts.push_front(Vector3(leftF, pts[first].y(), pts[first].z()));
            pts.push_back(Vector3(rightF, pts[last].y(), pts[last].z()));

        }else if(activeSurface == backNode) {
            if (pts[first].x() < pts[last].x()) {
                std::reverse(pts.begin(),pts.end());
            }
            pts.push_front(Vector3(rightF, pts[first].y(), pts[first].z()));
            pts.push_back(Vector3(leftF, pts[last].y(), pts[last].z()));
        }else if(activeSurface == leftNode) {
            if (pts[first].z() < pts[last].z()) {
                std::reverse(pts.begin(),pts.end());
            }
            pts.push_front(Vector3(pts[first].x(), pts[first].y(), farF));
            pts.push_back(Vector3(pts[last].x(), pts[last].y(), nearF));
        }else if(activeSurface == rightNode) {
            if (pts[first].z() > pts[last].z()) {
                std::reverse(pts.begin(),pts.end());
            }
            pts.push_front(Vector3(pts[first].x(), pts[first].y(), nearF));
            pts.push_back(Vector3(pts[last].x(), pts[last].y(), farF));
        }

        if (activeSurface != topNode && activeSurface != bottomNode) {
            activeSurface->opposite->spline.points.clear();
            foreach (Vector3 var, activeSurface->spline.points) {
                activeSurface->opposite->projectPoint(var);
            }

            Vector3 lastOpposite = activeSurface->opposite->spline.points[activeSurface->opposite->spline.points.size()-1];
            Vector3 firstOpposite = activeSurface->opposite->spline.points[0];
            activeSurface->right->spline.addPoint(firstOpposite);
            activeSurface->right->spline.addPoint(pts[0]);
            activeSurface->left->spline.addPoint(lastOpposite );
            activeSurface->left->spline.addPoint(pts[pts.size()-1]);
        }else {
            activeSurface->spline.points.clear();
            // TODO: clear suggestions and disable suggestions for this surface
        }

     activeSurface = NULL;
    }
}

void BoxNode::makeLayer() {


    QVector<Vector3> triangles;
    QVector4D c(0.1, 0.3, 0.4, 1.0);
    for (float i = 0.0;i<=0.9;i+=0.1) {
        Vector3 p1 = activeSurface->spline.getPoint(i);
        Vector3 p2 = activeSurface->opposite->spline.getPoint(i);
        Vector3 p3 = activeSurface->spline.getPoint(i+0.1);
        Vector3 p4 = activeSurface->opposite->spline.getPoint(i+0.1);
        triangles.push_back(p1);
        triangles.push_back(p2);
        triangles.push_back(p3);
        triangles.push_back(p3);
        triangles.push_back(p2);
        triangles.push_back(p4);
    }
    QVector<Vector3> outline;

    for (float i = 0.0;i<=1.0;i+=0.1) {
        outline.push_back(activeSurface->spline.getPoint(i));
    }
    for (float i = 0.0;i<=1.0;i+=0.1) {
        outline.push_back(activeSurface->right->spline.getPoint(i));
    }
    for (float i = 0.0;i<=1.0;i+=0.1) {
        outline.push_back(activeSurface->opposite->spline.getPoint(i));
    }
    for (float i = 0.0;i<=1.0;i+=0.1) {
        outline.push_back(activeSurface->left->spline.getPoint(i));
    }

    Surface * s = new Surface(triangles, outline, c);
    Node * n = new Node(s);
    children.append(n);

    foreach(SideNode* s, surfaces) {
        s->spline.points.clear();
    }
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

    foreach(Node * s, surfaces) {
        s->drawSpline();
        s->shape->drawLines();
    }
    foreach(Node * s, surfaces) {
        s->shape->drawShape();
    }

}
