#include "util.h"
#include "ridgenode.h"
#include "surfacenode.h"
#include "surface.h"
#include <QVector2D>

RidgeNode::RidgeNode(QVector<QVector2D> uv) : BaseNode("rigde"), uv(uv)
{
    smooth(this->uv);
    for (int i = 0; i<this->uv.size(); ++i) {
        float distanceFromMiddle = fabs(i-this->uv.size()/2.0)/this->uv.size();
        heights.push_back(0.5-distanceFromMiddle);
    }
    //repositionOnSurface(*parent);
}

QVector<Vector3> RidgeNode::intersectionPoints(Vector3 from, Vector3 direction) {
    if (active) {
        return BaseNode::intersectionPoints(from, direction);
    }else {
        QVector<Vector3> intersects = parent->intersectionPoints(from, direction);
        if (intersects.size() > 0) {
            for (float i = 0.0; i < 1.01;i+=0.1) {
                Vector3 p = spline.getPoint(i);
                if ((intersects[0] - p).lenght() < 0.5) {
                    return intersects;
                }
            }

            intersects.clear();
        }
        return intersects;
    }
}

void RidgeNode::addPoint(Vector3 from, Vector3 direction) {
    float s,t;

    QVector<Vector3> points = shape->intersectionPoints(from, direction, s, t);
    if (points.size() > 0) {
        sketchingSpline.addPoint(points[0]);
        uvSketch.push_back(QVector2D(s,t));
    }
}

void RidgeNode::determineActionOnStoppedDrawing() {
    //BaseNode::determineActionOnStoppedDrawing();
    sketchingSpline.clear();
    smooth(uvSketch);
    if (uvSketch.size() < 1) return;
    if (uvSketch[0].x() > uvSketch[uvSketch.size()-1].x()) {
        std::reverse(uvSketch.begin(), uvSketch.end());
    }
    for (int i= 0;i<uvSketch.size()-1;++i) {
        QVector2D p1 = uvSketch[i];
        QVector2D p2 = uvSketch[i+1];

        int begin = p1.x()*heights.size();
        int end = p2.x()*heights.size();

        for (int j = begin; j <=end; ++j) {
            if (j <heights.size()) {
                float t = j - begin;
                float nHeight = p1.y() * (1-t) + p2.y()*t;
                heights[j] = nHeight;

            }
        }

    }
    uvSketch.clear();

    SurfaceNode * surfaceParent = dynamic_cast<SurfaceNode *>(parent);
    surfaceParent->invalidate();

}

void RidgeNode::makeWall() {
    this->diffuse = QVector4D(0.5,0.3,0.3,0.5);
    QVector<vertex> triangles;

    Spline& spline = baseSpline;
    float size = spline.getPoints().size();

    for (int i = 0 ; i < spline.getPoints().size(); ++i) {
        const Vector3 & current = spline.getPoints()[i];
        Vector3 previous, next;
        if (i == 0) {
            previous = current;
        }else {
            previous = spline.getPoints()[i-1];
        }
        if (i == spline.getPoints().size() -1) {
            next = current;
        }else {
            next = spline.getPoints()[i+1];
        }
        Vector3 up(0, 5, 0);
        Vector3 normal = (next-previous).cross(up).normalize();
        Vector3 top = current+up;
        float u = i/size;
        triangles.push_back(vertex(current.x(), current.y(), current.z(), normal.x(), normal.y(), normal.z(), u, 0));
        triangles.push_back(vertex(top.x(), top.y(), top.z(), top.x(), top.y(), top.z(), u, 5));


    }/*
    for (int i = spline.points.size()-1 ; i >=0; --i) {
        Vector3 & current = spline.points[i];
        Vector3 previous, next;
        if (i == 0) {
            next = current;
        }else {
            next = spline.points[i-1];
        }
        if (i == spline.points.size() -1) {
            previous = current;
        }else {
            previous = spline.points[i+1];
        }
        Vector3 up(0, 5, 0);
        Vector3 normal = (next-previous).cross(up).normalize();

        triangles.push_back(current);
        triangles.push_back(current+up);
        normals.push_back(normal);
        normals.push_back(normal);


    }*/
    QVector<Vector3> noOutline;
    Surface* s = new Surface(triangles,noOutline,true);
    shape = s;
}

void RidgeNode::smooth(QVector<QVector2D>& uv) {
    if (uv.size()<1) return;
    QVector<QVector2D> newPoints;
    for (float p = 0; p <1.001;p+=0.01) {
        QVector<QVector2D> points = uv;
        for (int i = 1; i< points.size(); ++i) {
            for (int j = 0;j <points.size()-i;++j) {
                points[j] = points[j]*(1-p) + points[j+1]*p;
            }
        }
        newPoints.push_back(points[0]);
    }
    uv= newPoints;
}

void RidgeNode::doTransformSurface(QVector < QVector < Vector3 > > & rows, float resolution, float size) {
    if (baseSpline.getPoints().size() < 2 || spline.getPoints().size() < 2) return;

    float gridsize = 1.0/resolution;
    float worldToGrid = resolution/size;

    QVector < float > fill(rows[0].size());
    QVector < QVector < float > > heigths(rows.size(), fill);
    for (int z = 0; z<rows.size(); ++z) {
        for (int x = 0; x<rows[0].size();++x) {
            QVector2D point(x,z);

            for (int i = 0; i< uv.size()-1;++i) {
                QVector2D p1 = uv[i]/gridsize;
                QVector2D p2 = uv[i+1]/gridsize;
                float h1 = this->heights[i];
                float h2 = this->heights[i+1];
                QVector2D normal(p2.y()-p1.y(), -(p2.x()-p1.x()));
                QVector2D point2 = point +normal;

                QVector2D intersect = linesIntersection(p1,p2, point, point2);

                QVector2D a = intersect-p1;
                QVector2D b = p2-p1;
                float where = a.length()/b.length();
                if (((a.x() > 0 && b.x() > 0 )||(a.x() <= 0 && b.x() <= 0))) {

                    if (where <= 1.0) {
                        float heigth = h1*(1-where) + h2 *where;
                        //heigth*=20;
                        float influence = heigth*worldToGrid;
                        float dist = (point-intersect).length();
                        if (dist <= influence) {
                            float weigth = 1.0 - dist/influence;
                            float newheigth = clamp(heigth * weigth, 0, heigth-gridsize*size);
                            if (heigths[z][x] < newheigth)
                                heigths[z][x] = newheigth;
                        }
                    }else if (i < uv.size()-2) {
                        QVector2D p3 = uv[i+2]/gridsize;
                        //float h3 = this->heights[i+2];
                        QVector2D normal2(p3.y()-p2.y(), -(p3.x()-p2.x()));
                        QVector2D point3 = point +normal2;

                        QVector2D intersect2 = linesIntersection(p2,p3, point, point3);

                        QVector2D a2 = intersect2-p2;
                        QVector2D b2 = p2-p3;
                        //float where2 = a2.length()/b2.length();
                        if (((a2.x() >= 0 && b2.x() >= 0 )||(a2.x() <= 0 && b2.x() <= 0))) {

                            float l1 = (point - p2).length();
                            float influence1 = h2 * worldToGrid;
                            if (l1 <= influence1) {
                                float weigth = 1.0 - l1/influence1;
                                float newheigth = clamp(h2 * weigth, 0, h2-gridsize*size);
                                if (heigths[z][x] < newheigth)
                                    heigths[z][x] = newheigth;
                                continue;
                            }
//                            float l2 = (point - p2).length();
//                            float influence2 = h2 * worldToGrid;
//                            if (l2 <= influence2) {
//                                float weigth = 1.0 - l2/influence2;
//                                float newheigth = clamp(h2 * weigth, 0, h2-gridsize*size);
//                                if (heigths[z][x] < newheigth)
//                                    heigths[z][x] = newheigth;
//                                continue;
//                            }
                    }

                }



            }
        }
    }
    }
    for (int z = 0; z<rows.size(); ++z) {
        for (int x = 0; x<rows[0].size();++x) {
           //if (heigths[z][x] >= rows[z][x].y())
                rows[z][x] = Vector3(rows[z][x].x(),rows[z][x].y() + heigths[z][x],rows[z][x].z());
        }
    }


}


void RidgeNode::repositionOnSurface(SurfaceNode &surfacenode) {
    baseSpline.clear();
    spline.clear();
    for (int i = 0; i<uv.size(); ++i) {
        Vector3 point = surfacenode.getPointFromUv(uv[i]);
        baseSpline.addPoint(point);
        spline.addPoint(Vector3(point.x(), point.y() + heights[i], point.z()));

    }
    if (shape != NULL) {
        delete shape;
        shape = NULL;
    }

    makeWall();
}

void RidgeNode::drawSelf() {
    if (active)
        BaseNode::drawSelf();
    else
        drawSplines();
}

void RidgeNode::addSubclassJson(QVariantMap &map) {
    map["baseSpline"] = baseSpline.toJson();

    map["uv"] = Vector2DListToVariantList(uv);
    map["heights"] = FloatListToVariantList(heights);

}

void RidgeNode::fromJsonSubclass(QVariantMap map) {
    baseSpline.fromJson(map["baseSpline"].toMap());
    uv = variantToVector2DVector(map["uv"]);
    heights = variantToFloatVector(map["heights"]);
    //uvSketch = QVector<QVector2D>();
    active = false;

}
