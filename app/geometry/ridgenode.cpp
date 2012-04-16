#include "util.h"
#include "ridgenode.h"
#include "surfacenode.h"
#include "surface.h"
#include <QVector2D>

RidgeNode::RidgeNode(QVector<QVector2D> uv, SurfaceNode* parent) : BaseNode("rigde"), surfaceNode(parent), uv(uv)
{
    for (int i = 0; i<uv.size(); ++i) {
        Vector3 point = parent->getPointFromUv(uv[i]);
        baseSpline.addPoint(point);

        float distanceFromMiddle = fabs(i-uv.size()/2.0)/uv.size();
        heights.push_back(0.5-distanceFromMiddle);
        spline.addPoint(Vector3(point.x(), point.y() + heights[i], point.z()));

    }
}

QVector<Vector3> RidgeNode::intersectionPoints(Vector3 from, Vector3 direction) {
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

void RidgeNode::determineActionOnStoppedDrawing() {
    BaseNode::determineActionOnStoppedDrawing();
    SurfaceNode * surfaceParent = dynamic_cast<SurfaceNode *>(parent);
    surfaceParent->invalidate();

}

void RidgeNode::makeWall() {
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

float sideOfLine(QVector2D A, QVector2D B, QVector2D P) {
    return (B.x() - A.x()) * (P.y() - A.y()) - (B.y() - A.y()) * (P.x() - A.x());
}

bool inside(QVector2D A, QVector2D B, QVector2D C, QVector2D P, float* u, float*v) {
    // Compute vectors
    QVector2D v0 = C - A;
    QVector2D v1 = B - A;
    QVector2D v2 = P - A;

    // Compute dot products
    float dot00 = QVector2D::dotProduct(v0, v0);
    float dot01 = QVector2D::dotProduct(v0, v1);
    float dot02 = QVector2D::dotProduct(v0, v2);
    float dot11 = QVector2D::dotProduct(v1, v1);
    float dot12 = QVector2D::dotProduct(v1, v2);

    // Compute barycentric coordinates
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    *u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    *v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (*u >= 0) && (*v >= 0) && (*u + *v < 1);
}

bool inside(QVector2D A,QVector2D B,QVector2D C,QVector2D D, QVector2D point) {
    return (sideOfLine(A, B, point) >= 0) &&
           (sideOfLine(B, C, point) >= 0) &&
           (sideOfLine(C, D, point) >= 0) &&
           (sideOfLine(D, A, point) >= 0);
//    float u,v,w;
//    barycent(A,B,C,point,&u,&v,&w);
//    if (u>=0.0&&u<=1.0&&v>=0.0&&v<=1.0&&w>=0.0&&w<=1.0) {
//        return true;
//    }
//    barycent(A,C,D,point,&u,&v,&w);
//    if (u>=0.0&&u<=1.0&&v>=0.0&&v<=1.0&&w>=0.0&&w<=1.0) {
//        return true;
//    }
//    return false;
}

float lowest(float a, float b, float c, float d) {
    if (b < a)
        a = b;
    if (c < a)
        a = c;
    if (d < a)
        a = d;
    return a;
}

float highest(float a, float b, float c, float d) {
    if (b > a)
        a = b;
    if (c > a)
        a = c;
    if (d > a)
        a = d;
    return a;
}

QVector2D linesIntersection(QVector2D& p1, QVector2D& p2, QVector2D& p3, QVector2D& p4) {
    // Store the values for fast access and easy
    // equations-to-code conversion
    float x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
    float y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();

    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    // If d is zero, there is no intersection
    //if (d == 0) return NULL;

    // Get the x and y
    float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

    // Check if the x and y coordinates are within both lines
//    if ( x < min(x1, x2) || x > max(x1, x2) ||
//    x < min(x3, x4) || x > max(x3, x4) ) return NULL;
//    if ( y < min(y1, y2) || y > max(y1, y2) ||
//    y < min(y3, y4) || y > max(y3, y4) ) return NULL;

    // Return the point of intersection
    QVector2D ret(x,y);
    return ret;
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
                if (((a.x() > 0 && b.x() > 0 )||(a.x() <= 0 && b.x() <= 0)) && where <= 1.0) {

                    float heigth = h1*(1-where) + h2 *where;
                    //heigth*=20;
                    float influence = heigth*worldToGrid;
                    float dist = (point-intersect).length();
                    if (dist <= influence) {
                        float weigth = 1.0 - dist/influence;
                        heigth = heigth * weigth;
                        if (heigths[z][x] < heigth)
                            heigths[z][x] = heigth;
                    }

                } else {
                    float l1 = (point - p1).length();
                    float influence1 = h1 * worldToGrid;
                    if (l1 <= influence1) {
                        float weigth = 1.0 - l1/influence1;
                        h1 = h1 * weigth;
                        if (heigths[z][x] < h1)
                            heigths[z][x] = h1;
                        continue;
                    }
                    float l2 = (point - p2).length();
                    float influence2 = h2 * worldToGrid;
                    if (l2 <= influence2) {
                        float weigth = 1.0 - l2/influence2;
                        h2 = h2 * weigth;
                        if (heigths[z][x] < h1)
                            heigths[z][x] = h2;
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


void RidgeNode::repositionOnSurface(SurfaceNode &surfacenode) {

}

void RidgeNode::drawSelf() {
    if (active)
        BaseNode::drawSelf();
    else
        drawSplines();
}
