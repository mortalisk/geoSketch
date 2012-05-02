#include "basenode.h"
#include <GL/glew.h>
#include "surface.h"
#include "float.h"
#include <algorithm>

BaseNode::BaseNode(QString name) {
	shape = NULL;
        this->name = name;
        visible = true;

        diffuse.setX(0.5);
        diffuse.setY(0.5);
        diffuse.setZ(0.5);
        diffuse.setW(1.0);

        ambient.setX(0.5);
        ambient.setY(0.5);
        ambient.setZ(0.5);
        ambient.setW(1.0);
        proxy = NULL;
}

BaseNode::BaseNode(Shape *shape, QString name) {
	this->shape = shape;
        this->name = name;
        visible = true;

        diffuse.setX(0.5);
        diffuse.setY(0.5);
        diffuse.setZ(0.5);
        diffuse.setW(1.0);

        ambient.setX(0.5);
        ambient.setY(0.5);
        ambient.setZ(0.5);
        ambient.setW(1.0);

        proxy = NULL;
}


BaseNode::~BaseNode() {
    foreach(BaseNode* n,children) {
        delete n;
    }
}

void BaseNode::addPoint(Vector3 from, Vector3 direction) {
//    if (proxy) {
//        proxy->addPoint(from,direction);
//    }
//    else {
        QVector<Vector3> points = intersectionPoints(from, direction);
        if (points.size() > 0) {
            sketchingSpline.addPoint(points[0]);
        }
//    }

}

BaseNode * BaseNode::findIntersectingNode(Vector3 &from, Vector3 &direction, Vector3& point ) {
    BaseNode * found = NULL;
    float distance = FLT_MAX;
    foreach(BaseNode* child , children) {
        Vector3 p;
        BaseNode * foundLast = child->findIntersectingNode(from, direction,p);
        float distLast = (p - from).lenght();
        if (foundLast != NULL && distLast < distance) {
            found = foundLast;
            distance = distLast;
        }
    }
    if (found == NULL) {
        QVector<Vector3> points = intersectionPoints(from, direction);
        if (points.size() > 0) {
            point = points[0];
            return this;
        } else {
            return NULL;
        }
    } else {
        return found;
    }
}

void BaseNode::moveSketchingPointsToSpline() {
   spline.clear();
   spline.addAll(sketchingSpline.getPoints());
   sketchingSpline.clear();
}

void BaseNode::doOversketch() {
    if (sketchingSpline.getPoints().size() < 2) {
        sketchingSpline.clear();
        return;
    }
    Vector3 first = sketchingSpline.getPoints()[0];
    int nearestFirst = spline.findNearestPoint(first);
    Vector3 last = sketchingSpline.getPoints()[sketchingSpline.getPoints().size() - 1];
    int nearestLast = spline.findNearestPoint(last);

    oversketchSide(first, nearestFirst, true);
    oversketchSide(last, nearestLast, false);

    moveSketchingPointsToSpline();
}

void BaseNode::oversketchSide(Vector3& pointInSketch, int nearest, bool first) {

    if (isPointNearerSide(pointInSketch, nearest)) return;

    if (nearest != 0) {
        if (first) {
            for (int i = nearest; i >= 0; --i) {
                sketchingSpline.addPointFront(spline.getPoints()[i]);
            }
        }else {
            for (int i = nearest; i < spline.getPoints().size()-1; ++i) {

                sketchingSpline.addPoint(spline.getPoints()[i]);
            }
        }
    }
}

bool BaseNode::isPointNearerSide(Vector3& point, int indexInSpline) {
    return false;
}

void BaseNode::determineActionOnStoppedDrawing() {
    correctSketchingDirection();

    doOversketch();

}

void BaseNode::correctSketchingDirection() {
    bool isOpposite = spline.isLeftToRight() != sketchingSpline.isLeftToRight();

    if (isOpposite) {
        sketchingSpline.reverse();
    }


    sketchingSpline.smooth();
}

BaseNode * BaseNode::makeLayer() {
    return NULL;
}

QVector<Vector3> BaseNode::intersectionPoints(Vector3 from, Vector3 direction) {
	from = from - position;
	direction = direction - position;
	if (shape != NULL) {
		return shape->intersectionPoints(from, direction);
	} else {
		QVector<Vector3> empty;
		return empty;
	}
}

void BaseNode::drawChildren() {
        foreach(BaseNode* node, children) {
		node->draw();
	}
}
void BaseNode::drawSplines() {
    drawSpline(sketchingSpline, 1);
    drawSpline(spline, 0);
}

void BaseNode::drawSpline(Spline & spline, float r) {
        if (spline.getPoints().size() >= 1) {
                for (int i = 0; i < spline.getPoints().size() - 1; ++i) {

			glLineWidth(2.0f);
                        glPointSize(3.0f);
                        glBegin(GL_POINT);

                        float c[4] = {0.0,0.0,0.0,1.0};
                        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);
                        glVertex3f(spline.getPoints()[i].x(),spline.getPoints()[i].y(),spline.getPoints()[i].z());
                        glEnd();
			glBegin(GL_LINES);
                        glColor3f(r, 0, 0);
                        glVertex3d(spline.getPoints()[i].x(), spline.getPoints()[i].y(),
                                        spline.getPoints()[i].z());
                        glVertex3d(spline.getPoints()[i + 1].x(), spline.getPoints()[i + 1].y(),
                                        spline.getPoints()[i + 1].z());
			glEnd();
		}
	}
}

void BaseNode::drawSelf() {

	glTranslatef(position.x(), position.y(), position.z());

	if (shape != NULL) {
                shape->drawLines(!visible);
                if (visible) {
                    QVector4D color = diffuse;
                    if (active) {
                        color = QVector4D(1.0,0.0,0.0,color.w());
                    }
                    shape->drawShape(ambient, color);
                }
	}
        drawSplines();

}

void BaseNode::draw() {


    glPushMatrix();

    prepareForDrawing();

    drawSelf();
    drawChildren();




    glPopMatrix();


}
