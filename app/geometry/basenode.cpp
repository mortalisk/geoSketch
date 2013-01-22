#include "basenode.h"
#include <GL/glew.h>
#include "surface.h"
#include "float.h"
#include <algorithm>
#include "util.h"
#include <QVector2D>
#include <QMenu>
#include "scene.h"
#include <QTextStream>
#include "boxnode.h"

BaseNode::BaseNode(QString name) : QObject() {
    init();
    this->name = name;
    this->diffuse = QVector4D(0.5,0.5,0.5,1.0);
}

void BaseNode::init() {
    parent = NULL;
    shape = NULL;
    visible = true;
    active = false;

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
    float ss, tt;
        QVector<Vector3> points = intersectionPoints(from, direction,ss,tt);
        if (points.size() > 0) {
            sketchingSpline.addPoint(points[0]);
            uvSketchingSpline.addPoint(QVector2D(ss,tt));
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
        float ss, tt;
        QVector<Vector3> points = intersectionPoints(from, direction,ss,tt);
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
   sketchingSpline.clear();

   uvSpline.clear();
   uvSpline.addAll(uvSketchingSpline.getPoints());
   uvSketchingSpline.clear();

   for(int i = 0; i<uvSpline.getPoints().size(); ++i) {
       spline.addPoint(getPointFromUv(uvSpline.getPoints()[i]));
   }
}

void BaseNode::doOversketch() {
    if (uvSketchingSpline.getPoints().size() < 2) {
        uvSketchingSpline.clear();
        sketchingSpline.clear();
        return;
    }
    QVector2D first = uvSketchingSpline.getPoints()[0];
    int nearestFirst = uvSpline.findNearestPoint(first);
    QVector2D last = uvSketchingSpline.getPoints()[uvSketchingSpline.getPoints().size() - 1];
    int nearestLast = uvSpline.findNearestPoint(last);

    oversketchSide(first, nearestFirst, true);
    oversketchSide(last, nearestLast, false);

    moveSketchingPointsToSpline();
}

void BaseNode::addActions(QToolBar * menu) {
    QList<QAction*> opts;

    QAction * actionDelete = new QAction("Delete", menu);
    opts.append(actionDelete);
    connect(actionDelete,SIGNAL(triggered()), this, SLOT(deleteItem()));

    QAction * actionClear = new QAction("Clear", menu);
    opts.append(actionClear);
    connect(actionClear,SIGNAL(triggered()), this, SLOT(clearSketch()));

    menu->addActions(opts);

    menu->addSeparator();
    addSubclassActions(menu);
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

void BaseNode::oversketchSide(QVector2D& pointInSketch, int nearest, bool first) {

    if (isPointNearerSide(pointInSketch, nearest)) return;

    if (nearest != 0) {
        if (first) {
            for (int i = nearest; i >= 0; --i) {
                uvSketchingSpline.addPointFront(uvSpline.getPoints()[i]);
            }
        }else {
            for (int i = nearest; i < uvSpline.getPoints().size()-1; ++i) {

                uvSketchingSpline.addPoint(uvSpline.getPoints()[i]);
            }
        }
    }
}

bool BaseNode::isPointNearerSide(QVector2D&, int) {
    return false;
}

void BaseNode::determineActionOnStoppedDrawing() {
    correctSketchingDirection();

    doOversketch();

}

void BaseNode::correctSketchingDirection() {
    bool isOpposite = uvSpline.isLeftToRight() != uvSketchingSpline.isLeftToRight();

    if (isOpposite) {
        uvSketchingSpline.reverse();
    }


    uvSketchingSpline.smooth();
}

QVector<Vector3> BaseNode::intersectionPoints(Vector3 from, Vector3 direction, float&s, float&t) {
	from = from - position;
    direction = direction - position;
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

                        float c[4] = {r,0.0,0.0,1.0};
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
                shape->drawLines(!visible, active?1:0,0,0,1);
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

void BaseNode::makeObj(QTextStream& s, QTextStream &materials, int* objNo) {


    BoxNode * bn = qobject_cast<BoxNode*>(this);
    if(bn) {
        bn->waterNode->makeObj(s, materials, objNo);
//        bn->frontNode->makeObj(s, materials, objNo);


//        bn->topNode->makeObj(s, materials, objNo);
//        bn->bottomNode->makeObj(s, materials, objNo);
//        bn->backNode->makeObj(s, materials, objNo);

//        bn->leftNode->makeObj(s, materials, objNo);

//        bn->rightNode->makeObj(s, materials, objNo);
    }
    if (shape != NULL && visible && includeShapeInExport()) {
         *objNo = *objNo + 1;
        s << "usemtl Color" << name << *objNo << "\n";
        s << "o [" << name << *objNo << "]" << "\n";
        materials << "newmtl Color" << name << *objNo << "\n";
        materials << "Tr " << ambient.w() << "\n";
        materials << "Ka " << ambient.x() << " " << ambient.y() << " " << ambient.z() << "\n";
        materials << "Kd " << diffuse.x() << " " << diffuse.y() << " " << diffuse.z() << "\n";
        materials << "Ks 0 0 0\n";
        if(shape->strip) {
            for (int i = 0; i< shape->triangles.size(); i++) {
                vertex & v1 = shape->triangles[i];
                s << "v " << v1.x << " " << v1.y << " "<< v1.z << "\n";
                if (i >= 2) {
                    s << "f -3 -2 -1" << "\n";
                }

            }
        }else {
            for(int i = 0; i< shape->triangles.size(); i+=3) {
                vertex & v1 = shape->triangles[i];
                vertex & v2 = shape->triangles[i+1];
                vertex & v3 = shape->triangles[i+2];

                s << "v " << v1.x << " " << v1.y << " "<< v1.z << "\n";
                s << "v " << v2.x << " " << v2.y << " "<< v2.z << "\n";
                s << "v " << v3.x << " " << v3.y << " "<< v3.z << "\n";
                s << "f -3 -2 -1" << "\n";
            }
        }
    }
    foreach(BaseNode * b,children) {
        b->makeObj(s, materials, objNo);
    }
}

void BaseNode::draw() {


    glPushMatrix();

    prepareForDrawing();

    drawSelf();
    drawSplines();
    drawChildren();




    glPopMatrix();


}


