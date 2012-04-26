#include "util.h"
#include "rivernode.h"
#include "surfacenode.h"
#include "surface.h"
#include <QVector2D>

RiverNode::RiverNode(QVector<QVector2D> uv, SurfaceNode* parent) : BaseNode("rigde"), surfaceNode(parent), uv(uv)
{
    for (int i = 0; i<uv.size(); ++i) {
        float distanceFromMiddle = fabs(i-uv.size()/2.0)/uv.size();
        heights.push_back(0.5-distanceFromMiddle);
    }
    repositionOnSurface(*parent);
}

QVector<Vector3> RiverNode::intersectionPoints(Vector3 from, Vector3 direction) {
    if (active) {
        return BaseNode::intersectionPoints(from, direction);
    }else {
        QVector<Vector3> intersects = parent->intersectionPoints(from, direction);
        if (intersects.size() > 0) {
            for (float i = 0.0; i < 1.01;i+=0.1) {
                Vector3 p = spline.getPoint(i);
                if ((intersects[0] - p).lenght() < 1.0) {
                    return intersects;
                }
            }

            intersects.clear();
        }
        return intersects;
    }
}

void RiverNode::determineActionOnStoppedDrawing() {
    BaseNode::determineActionOnStoppedDrawing();
    SurfaceNode * surfaceParent = dynamic_cast<SurfaceNode *>(parent);
    surfaceParent->invalidate();

}

void RiverNode::makeWall() {
    this->diffuse = QVector4D(0.5,0.3,0.3,0.5);
    QVector<Vector3> triangles;
    QVector<Vector3> normals;

    Spline& spline = baseSpline;

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
        triangles.push_back(current);
        triangles.push_back(current+up);
        normals.push_back(normal);
        normals.push_back(normal);


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
    Surface* s = new Surface(triangles,normals, noOutline,true);
    shape = s;
}


void RiverNode::doTransformSurface(QVector < QVector < Vector3 > > & rows, float resolution, float size) {
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
                if (((a.x() > 0 && b.x() > 0 )||(a.x() <= 0 && b.x() <= 0)) && where <= 1.0) {

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

                } else {
                    float l1 = (point - p1).length();
                    float influence1 = h1 * worldToGrid;
                    if (l1 <= influence1) {
                        float weigth = 1.0 - l1/influence1;
                        float newheigth = clamp(h1 * weigth, 0, h1-gridsize*size);
                        if (heigths[z][x] < newheigth)
                            heigths[z][x] = newheigth;
                        continue;
                    }
                    float l2 = (point - p2).length();
                    float influence2 = h2 * worldToGrid;
                    if (l2 <= influence2) {
                        float weigth = 1.0 - l2/influence2;
                        float newheigth = clamp(h2 * weigth, 0, h2-gridsize*size);
                        if (heigths[z][x] < newheigth)
                            heigths[z][x] = newheigth;
                        continue;
                    }
                }



            }
        }
    }
    for (int z = 0; z<rows.size(); ++z) {
        for (int x = 0; x<rows[0].size();++x) {
            rows[z][x] = Vector3(rows[z][x].x(),rows[z][x].y()+heigths[z][x],rows[z][x].z());
        }
    }


}


void RiverNode::repositionOnSurface(SurfaceNode &surfacenode) {
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
        makeWall();
    }
}

void RiverNode::drawSelf() {
    if (active)
        BaseNode::drawSelf();
    else
        drawSplines();
}
