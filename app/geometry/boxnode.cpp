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
    farF = -depth/2;
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
    
    frontNode = new SideNode(A, B, C, D);
    backNode = new SideNode(F, E, H, G );
    topNode = new SideNode(D, C, G, H);
    bottomNode = new SideNode(E, F, B, A);
    leftNode = new SideNode(E, A, D, H);
    rightNode = new SideNode(B, F, G, C);

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
        candidate->sketchingSpline.addPoint(candidatePoint);


        activeSurface = candidate;
    }
}

void BoxNode::determineActionOnStoppedDrawing() {

	if (activeSurface) {

            if (activeSurface->spline.points.size() == 0) {
                activeSurface->moveSketchingPointsToSpline();
                activeSurface->makeSuggestionLines();
                makeSuggestionFor(activeSurface);
            } else {
                activeSurface->doOversketch();
            }

            activeSurface = NULL;
	}
}

void BoxNode::makeSuggestionFor(SideNode* side) {


    Vector3 first = side->spline.points[0];
    Vector3 last = side->spline.points[side->spline.points.size()-1];

    if(side==frontNode) {
        if (first.x() < last.x()) {
            Vector3 vectorOne = Vector3(leftF,first.y(),nearF);
            Vector3 vectorTwo = Vector3(rightF,last.y(),nearF);
            side->spline.points.push_front(vectorOne);
            side->spline.points.push_back(vectorTwo);
        }else {
            Vector3 vectorOne = Vector3(leftF,last.y(),nearF);
            Vector3 vectorTwo = Vector3(rightF,first.y(),nearF);
            side->spline.points.push_front(vectorTwo);
            side->spline.points.push_back(vectorOne);
        }
    }else if(side == backNode){
        if (first.x() < last.x()) {
            Vector3 vectorOne = Vector3(leftF,first.y(),farF);
            Vector3 vectorTwo = Vector3(rightF,last.y(),farF);
            side->spline.points.push_front(vectorOne);
            side->spline.points.push_back(vectorTwo);
        }else {
            Vector3 vectorOne = Vector3(leftF,last.y(),farF);
            Vector3 vectorTwo = Vector3(rightF,first.y(),farF);
            side->spline.points.push_front(vectorTwo);
            side->spline.points.push_back(vectorOne);
        }
    }else if(side == leftNode){
            if (first.z() < last.z()) {
                Vector3 vectorOne = Vector3(leftF,first.y(),farF);
                Vector3 vectorTwo = Vector3(leftF,last.y(),nearF);
                side->spline.points.push_front(vectorOne);
                side->spline.points.push_back(vectorTwo);
            }else {
                Vector3 vectorOne = Vector3(leftF,last.y(),farF);
                Vector3 vectorTwo = Vector3(leftF,first.y(),nearF);
                side->spline.points.push_front(vectorTwo);
                side->spline.points.push_back(vectorOne);
            }
        }else if(side == rightNode){
        if (first.z() < last.z()) {
            Vector3 vectorOne = Vector3(rightF,first.y(),farF);
            Vector3 vectorTwo = Vector3(rightF,last.y(),nearF);
            side->spline.points.push_front(vectorOne);
            side->spline.points.push_back(vectorTwo);
        }else {
            Vector3 vectorOne = Vector3(rightF,last.y(),farF);
            Vector3 vectorTwo = Vector3(rightF,first.y(),nearF);
            side->spline.points.push_front(vectorTwo);
            side->spline.points.push_back(vectorOne);
        }
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
        s->drawSplines();
        s->shape->drawLines();
    }
    foreach(Node * s, surfaces) {
        s->shape->drawShape();
    }

}
