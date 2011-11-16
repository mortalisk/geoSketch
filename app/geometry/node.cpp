#include "node.h"
#include <GL/glew.h>
#include "surface.h"
#include "float.h"

Node::Node() {
	shape = NULL;
}

Node::Node(Shape *shape) {
	this->shape = shape;
}


Node::~Node() {
    foreach(Node* n,children) {
        delete n;
    }
}

void Node::addPoint(Vector3 from, Vector3 direction) {

	QVector<Vector3> points = intersectionPoints(from, direction);
	Vector3 & pos = points[0];
	if (spline.length() > 0 && (spline.lastPoint() - pos).lenght() < 0.5) {
		spline.changeLastPoint(pos);
		return;
	} else {
		spline.addPoint(pos);
	}

}

void Node::doOversketch() {
	if (sketchingSpline.points.size() < 2)
		return;

	Vector3 first = sketchingSpline.points[0];
	Vector3 last = sketchingSpline.points[sketchingSpline.points.size() - 1];

        int nearestFirst = spline.findNearestPoint(first);

        int nearestLast = spline.findNearestPoint(last);

        bool isOpposite = spline.isRightToLeft() != sketchingSpline.isRightToLeft();

        if (isOpposite) {
            if (nearestLast != 0) {
                for(int i = nearestLast; i >= 0; --i) {
                    sketchingSpline.points.push_back(spline.points[i]);
                }
            }
            if (nearestFirst < spline.points.size() - 1) {
                for (int i = nearestFirst; i < spline.points.size(); ++i) {
                    sketchingSpline.points.push_front(spline.points[i]);
                }
            }
        }else {
            if (nearestFirst != 0) {
                for (int i = nearestFirst; i >= 0; --i) {
                    sketchingSpline.points.push_front(spline.points[i]);
                }
            }
            if (nearestLast < spline.points.size() - 1) {
                for (int i = nearestLast; i < spline.points.size(); ++i) {
                    sketchingSpline.points.push_back(spline.points[i]);
                }
            }
        }

	spline.points.clear();
	spline.points += sketchingSpline.points;
	sketchingSpline.points.clear();
}

void Node::determineActionOnStoppedDrawing() {

}

void Node::makeLayer() {

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
	glPopMatrix();

}

void Node::draw() {

	drawChildren();

	drawSelf();

}
