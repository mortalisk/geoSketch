#include "boxnode.h"

#include <iostream>
#include <algorithm>
#include "GL/glew.h"
#include "surface.h"
#include "float.h"
#include "surfacenode.h"

BoxNode::BoxNode() : Node::Node("boxnode")
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

    setUpSurfaces();

}

BoxNode::BoxNode(BoxNode& o): Node::Node(o) {
    frontNode = new SideNode(*o.frontNode);
    backNode = new SideNode(*o.backNode);
    leftNode = new SideNode(*o.leftNode);
    rightNode = new SideNode(*o.rightNode);
    topNode = new SideNode(*o.topNode);
    bottomNode = new SideNode(*o.bottomNode);
    foreach(Node * child, o.children) {
        children.append(child->copy());
    }

    setUpSurfaces();

    activeSurface = NULL;
}

void BoxNode::setUpSurfaces() {
    frontNode->setOpposite(backNode);
    leftNode->setOpposite(rightNode);
    topNode->setOpposite(bottomNode);

    frontNode->setLeft(leftNode);
    leftNode->setLeft(backNode);
    backNode->setLeft(rightNode);
    rightNode->setLeft(frontNode);

    surfaces.push_back(frontNode);
    surfaces.push_back(backNode);
    surfaces.push_back(leftNode);
    surfaces.push_back(rightNode);
    surfaces.push_back(topNode);
    surfaces.push_back(bottomNode);

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

    // we must find the nearest intersection point
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
            activeSurface->correctSketchingDirection();

            if (activeSurface->spline.points.size() == 0) {
                activeSurface->moveSketchingPointsToSpline();
            } else {
                activeSurface->doOversketch();
            }

            activeSurface->makeSuggestionLines();

            makeSuggestionFor(activeSurface);

            activeSurface = NULL;
	}
}

void BoxNode::makeSuggestionFor(SideNode* side) {


    if (side->opposite->spline.isSuggestion) {
        side->opposite->spline.points.clear();

        // project points from this side to opposite
        Vector3 direction = side->opposite->lowerRigth - side->lowerLeft;
        side->opposite->projectPoints(direction, side->spline.points);

        side->opposite->spline.isSuggestion = true;
    }

    Vector3 left = side->spline.getLeftPoint();
    Vector3 right = side->spline.getRightPoint();
    Vector3 leftOpposite = side->opposite->spline.getLeftPoint();
    Vector3 rightOpposite = side->opposite->spline.getRightPoint();

    side->left->addInterpolatedSuggestion(rightOpposite.y(), left.y());
    side->right->addInterpolatedSuggestion(right.y(), leftOpposite.y());

    frontNode->ensureLeftToRigth();
    rightNode->ensureLeftToRigth();
    backNode->ensureLeftToRigth();
    leftNode->ensureLeftToRigth();

    side->spline.isSuggestion = false;

}

Node* BoxNode::makeLayer() {

    if (frontNode->spline.points.size() < 1)
        return this;

    SurfaceNode * n = new SurfaceNode( "Layer", frontNode->spline, rightNode->spline, backNode->spline, leftNode->spline);
    children.append(n);

    foreach(SideNode* s, surfaces) {
        s->spline.points.clear();
        s->spline.isSuggestion = true;
    }

    return n;
}

void BoxNode::draw() {
    glDisable(GL_CULL_FACE);


    drawChildren();

    glColor4f(0.5,0.5,0.5,1.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    drawSelf();
    glDisable(GL_CULL_FACE);
}

void BoxNode::drawSelf() {
    //Node::drawSelf();
    glTranslatef(position.x(), position.y(), position.z());

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    foreach(Node * s, surfaces) {
        s->drawSplines();
        s->shape->drawLines();
    }
    foreach(Node * s, surfaces) {
        glColor4f(0.5,0.5,0.5,0.5);
        s->shape->drawShape(ambient, diffuse);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}
