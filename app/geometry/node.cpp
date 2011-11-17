#include "node.h"
#include <GL/glew.h>
#include "surface.h"
#include "float.h"
#include <algorithm>

Node::Node(QString name) {
	shape = NULL;
        this->name = name;
}

Node::Node(Shape *shape, QString name) {
	this->shape = shape;
        this->name = name;
}


Node::~Node() {
    foreach(Node* n,children) {
        delete n;
    }
}

void Node::addPoint(Vector3 from, Vector3 direction) {

        QVector<Vector3> points = intersectionPoints(from, direction);
        sketchingSpline.addPoint(points[0]);

}

void Node::moveSketchingPointsToSpline() {
   spline.points.clear();
   spline.points += sketchingSpline.points;
   sketchingSpline.points.clear();
}

void Node::doOversketch() {
    if (sketchingSpline.points.size() < 2) {
        sketchingSpline.points.clear();
        return;
    }
    Vector3 first = sketchingSpline.points[0];
    int nearestFirst = spline.findNearestPoint(first);
    Vector3 last = sketchingSpline.points[sketchingSpline.points.size() - 1];
    int nearestLast = spline.findNearestPoint(last);

    oversketchSide(first, nearestFirst, true);
    oversketchSide(last, nearestLast, false);

    moveSketchingPointsToSpline();
}

void Node::oversketchSide(Vector3& pointInSketch, int nearest, bool first) {

    if (isPointNearerSide(pointInSketch, nearest)) return;

    if (nearest != 0) {
        if (first) {
            for (int i = nearest; i >= 0; --i) {
                sketchingSpline.points.push_front(spline.points[i]);
            }
        }else {
            for (int i = nearest; i < spline.points.size()-1; ++i) {

                sketchingSpline.points.push_back(spline.points[i]);
            }
        }
    }
}

bool Node::isPointNearerSide(Vector3& point, int indexInSpline) {
    return false;
}

void Node::determineActionOnStoppedDrawing() {
    correctSketchingDirection();

    doOversketch();

}

void Node::correctSketchingDirection() {

    bool isOpposite = spline.isLeftToRight() != sketchingSpline.isLeftToRight();

    if (isOpposite) {
        std::reverse(sketchingSpline.points.begin(), sketchingSpline.points.end());
    }
}

Node * Node::makeLayer() {

}

QVector<Vector3> Node::intersectionPoints(Vector3 from, Vector3 direction) {
	from = from - position;
	direction = direction - position;
	if (shape != NULL) {
		return shape->intersectionPoints(from, direction);
	} else {
		QVector<Vector3> empty;
		return empty;
	}
}

void Node::drawChildren() {
	foreach(Node* node, children) {
		node->draw();
	}
}
void Node::drawSplines() {
    drawSpline(sketchingSpline);
    drawSpline(spline);
}

void Node::drawSpline(Spline & spline) {
	if (spline.points.size() >= 1) {
		for (int i = 0; i < spline.points.size() - 1; ++i) {

			glLineWidth(2.0f);

			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex3d(spline.points[i].x(), spline.points[i].y(),
					spline.points[i].z());
			glVertex3d(spline.points[i + 1].x(), spline.points[i + 1].y(),
					spline.points[i + 1].z());
			glEnd();
		}
	}
}

void Node::drawSelf() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());

	if (shape != NULL) {
		shape->drawLines();
		shape->drawShape();
	}
        drawSplines();
	glPopMatrix();

}

void Node::draw() {

	drawChildren();

	drawSelf();

}
