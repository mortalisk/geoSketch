#include "basenode.h"
#include <GL/glew.h>
#include "surface.h"
#include "float.h"
#include <algorithm>
#include "util.h"
#include <QVector2D>
#include <QMenu>
#include "scene.h"

BaseNode::BaseNode(QString name) : QObject() {
    init();
    this->name = name;
    this->diffuse = QVector4D(0.5,0.5,0.5,1.0);
}

void BaseNode::init() {
    parent = NULL;
    shape = NULL;
    visible = true;

//    diffuse.setX(0.5);
//    diffuse.setY(0.5);
//    diffuse.setZ(0.5);
//    diffuse.setW(1.0);

    ambient.setX(0.5);
    ambient.setY(0.5);
    ambient.setZ(0.5);
    ambient.setW(1.0);
    proxy = NULL;

}

BaseNode::BaseNode(Shape *shape, QString name) : QObject() {
    init();
    this->shape = shape;
    this->name = name;
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

void BaseNode::addActions(QMenu * menu) {
    QList<QAction*> opts;
    addSubclassActions(menu);

    QAction * actionDelete = new QAction("delete", menu);
    opts.append(actionDelete);
    connect(actionDelete,SIGNAL(triggered()), this, SLOT(deleteItem()));

    menu->addActions(opts);
}

void BaseNode::deleteChild(BaseNode *child) {
    for (int i=0; i< children.size(); i++) {
        if (children[i] == child) {
            children.remove(i);
            delete child;
            childDeleted(child);
            i++;
            invalidate();
        }

    }
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

bool BaseNode::isPointNearerSide(Vector3&, int) {
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
    float s, t;
	if (shape != NULL) {
        return shape->intersectionPoints(from, direction,s,t);
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
                        glVertex3d(spline.getPoints()[i].x(), spline.getPoints()[i].y()+0.005,
                                        spline.getPoints()[i].z());
                        glVertex3d(spline.getPoints()[i + 1].x(), spline.getPoints()[i + 1].y()+0.005,
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
                        color = QVector4D(color.x()+0.3,color.y(),color.z(),color.w());
                    }
                    shape->drawShape(ambient, color);
                }
	}


}

QVariantMap BaseNode::toJson() {
    QVariantMap map;
    map["name"] = name;
    map["type"] = getTypeId();
    map["position"] = position.toJson();
    map["colorr"] = diffuse.x();
    map["colorg"] = diffuse.y();
    map["colorb"] = diffuse.z();
    map["colora"] = diffuse.w();
    QVariantList children;
    foreach(BaseNode* child,this->children) {
        QVariantMap c = child->toJson();
        children.append(c);
    }
    map["children"] = children;
    map["spline"] = spline.toJson();


    addSubclassJson(map);
    return map;
}

void BaseNode::fromJson(QVariantMap map) {
    name = map["name"].toString();
    position.fromJson(map["position"].toMap());
    diffuse.setX(map["colorr"].toDouble());
    diffuse.setY(map["colorg"].toDouble());
    diffuse.setZ(map["colorb"].toDouble());
    diffuse.setW(map["colora"].toDouble());
    foreach(QVariant var, map["children"].toList()) {
        QVariantMap cmap = var.toMap();
        QString type = cmap["type"].toString();
        BaseNode * c = create(type);
        c->fromJson(cmap);
        children.append(c);
        c->parent = this;
    }
    init();
    fromJsonSubclass(map);
}

void BaseNode::draw() {


    glPushMatrix();

    prepareForDrawing();

    drawSelf();
    drawSplines();
    drawChildren();




    glPopMatrix();


}


