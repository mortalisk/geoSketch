#include "boxnode.h"

#include <iostream>
#include <algorithm>
#include "GL/glew.h"
#include "surface.h"
#include "float.h"

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


    QVector<Vector3> triangles;
    QVector4D c(0.1, 0.3, 0.4, 1.0);
    QVector<Vector3> previousRow;

    Vector3 frontLeft = leftNode->spline.getPoint(1.0);
    Vector3 frontRight = rightNode->spline.getPoint(0.0);
    Vector3 backLeft = leftNode->spline.getPoint(0.0);
    Vector3 backRight = rightNode->spline.getPoint(1.0);

    for (double zi = 0.0;zi<=1.01;zi+=0.1) {
        QVector<Vector3> row;
        Vector3 rowLeft = frontLeft*(1.0-zi) + backLeft *(zi);
        Vector3 rowRigth = frontRight*(1.0-zi) + backRight * (zi);

        Vector3 left = leftNode->spline.getPoint(1.0-zi);
        Vector3 right = rightNode->spline.getPoint(zi);
        for (double xi = 0.0;xi<=1.01;xi+=0.1) {
            Vector3 colInt = rowLeft * (1.0-xi) + rowRigth * xi;
            Vector3 front = frontNode->spline.getPoint(xi);
            Vector3 back = backNode->spline.getPoint(1.0-xi);
            Vector3 frontBack = front*(1.0-zi)+back*zi;
            Vector3 diff = frontBack - colInt;

            Vector3 leftRight = left*(1.0-xi) + right*xi;
            Vector3 point = leftRight + diff;
            row.push_back(point);
        }
        if (previousRow.size() == row.size()) {
            for (int i = 0; i < previousRow.size()-1; ++i) {
                Vector3 & a = previousRow[i];
                Vector3 & b = previousRow[i+1];
                Vector3 & c = row[i];
                Vector3 & d = row[i+1];
                triangles.push_back(a);
                triangles.push_back(b);
                triangles.push_back(c);
                triangles.push_back(b);
                triangles.push_back(d);
                triangles.push_back(c);
            }
        }
        previousRow = row;
    }
    QVector<Vector3> outline;

    for (double i = 0.0;i<=1.01;i+=0.1) {
        outline.push_back(frontNode->spline.getPoint(i));
    }
    for (double i = 0.0;i<=1.01;i+=0.1) {
        outline.push_back(rightNode->spline.getPoint(i));
    }
    for (double i = 0.0;i<=1.01;i+=0.1) {
        outline.push_back(backNode->spline.getPoint(i));
    }
    for (double i = 0.0;i<=1.01;i+=0.1) {
        outline.push_back(leftNode->spline.getPoint(i));
    }

    Surface * s = new Surface(triangles, outline, c);
    Node * n = new Node(s, "Layer");
    children.append(n);

    foreach(SideNode* s, surfaces) {
        s->spline.points.clear();
    }

    return n;
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
