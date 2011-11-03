#include "node.h"
#include <GL/glew.h>
#include "surface.h"

Node::Node()
{
    shape = NULL;
}

Node::Node(Shape *shape) {
    this->shape = shape;
}

Node::~Node()
{
}

void Node::addPoint(Vector3 from, Vector3 direction) {

    QVector<Vector3> points = intersectionPoints(from, direction);
    Vector3 & pos = points[0];
    if (spline.length() > 0 && (spline.lastPoint() - pos).lenght() < 0.5) {
        spline.changeLastPoint(pos);
        return;
    }else {
        spline.addPoint(pos);
    }

}

void Node::stopDrawing() {

}

void Node::makeLayer() {

}

QVector<Vector3> Node::intersectionPoints(Vector3 from,Vector3 direction) {
    from = from - position;
    direction = direction - position;
    if (shape != NULL) {
        return shape->intersectionPoints(from, direction);
    }
    else {
        QVector<Vector3> empty;
        return empty;
    }
}

void Node::drawChildren() {
    foreach(Node* node, children) {
        node->draw();
    }
}

void Node::drawSpline() {
    if (spline.points.size() >= 1) {
        for (int i= 0; i<spline.points.size()-1; ++i) {

            glLineWidth(2.0f);

            glBegin(GL_LINES);
            glColor3f(0,0,0);
            glVertex3d(spline.points[i].x(), spline.points[i].y(), spline.points[i].z());
            glVertex3d(spline.points[i+1].x(), spline.points[i+1].y(), spline.points[i+1].z());
            glEnd();
        }
    }
}

void Node::drawSelf() {




    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());

    if (shape != NULL) {
        shape->drawLines();
        shape->drawShape();
    }
    glPopMatrix();

}

void Node::draw() {

    drawChildren();

    drawSelf();

}
