#include "node.h"
#include <GL/glew.h>
#include "surface.h"

Node::Node()
{
    foreach(Spline* s , splines) {
        delete s;
    }
}

void Node::addPoint(Vector3& pos) {
    std::cout << "Node: going to add new point" << std::endl;
    int activeSpline = splines.size();
    if (activeSpline < 1) {
        addSpline();
    } else {
        activeSpline--;
    }
    Spline* spline = splines[activeSpline];

    if (spline->length() > 0 && (spline->lastPoint() - pos).lenght() < 0.5) {
        spline->changeLastPoint(pos);
        return;
    }

    spline->addPoint(pos);

}

void Node::makeLayer() {

    Node * layerNode = new Node();
    layerNode->shape = new Surface(splines);
    children.append(layerNode);
    splines.clear();
}

void Node::addSpline() {
    splines.append(new Spline());
}

QVector<Vector3> Node::intersectionPoints(Vector3 from,Vector3 direction) {
    from = from - position;
    direction = direction - position;
    return shape->intersectionPoints(from, direction);
}

void Node::drawChildren() {
    foreach(Node* node, children) {
        node->draw();
    }
}

void Node::drawSelf() {

    foreach (Spline* spline, splines) {
        for(int i = 0; i < spline->points.size(); ++i) {
            glPushMatrix();
            glTranslatef(spline->points[i].x(),spline->points[i].y(),spline->points[i].z());
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            //glCallList(scene.cursorSphere->displayList);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
        }
        if (spline->points.size() >= 4) {
            for (int i= 1; i<spline->points.size()-2; ++i) {

                glLineWidth(5.0f);
                glBegin(GL_LINES);
                glColor3f(0,0,0);
                float t = 0.0f;
                do  {
                    Vector3 a = spline->katmullRom(t,spline->points[i-1],spline->points[i],spline->points[i+1],spline->points[i+2]);
                    t+=0.1f;
                    Vector3 b = spline->katmullRom(t,spline->points[i-1],spline->points[i],spline->points[i+1],spline->points[i+2]);
                    glVertex3f(a.x(),a.y(),a.z());
                    glVertex3f(b.x(),b.y(),b.z());
                } while (t < 0.99f);

                glEnd();
            }
        }
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());

    shape->draw();

}

void Node::draw() {

    drawChildren();

    drawSelf();

}
