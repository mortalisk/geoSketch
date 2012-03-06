#include "ridgenode.h"
#include "surfacenode.h"
#include "surface.h"

RidgeNode::RidgeNode(Spline& spline) : BaseNode("rigde")
{
    this->spline.points += spline.points;
    this->baseSpline.points += spline.points;
    for(int i = 0;i< spline.points.size(); ++i) {
        float distanceFromMiddle = fabs(i-spline.points.size()/2.0)/spline.points.size();
        Vector3 & point = this->spline.points[i];
        point = Vector3(point.x(), point.y()+0.5-distanceFromMiddle, point.z());
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

    QVector<Vector3> triangles;
    QVector<Vector3> normals;

    Spline& spline = baseSpline;

    for (int i = 0 ; i < spline.points.size(); ++i) {
        Vector3 & current = spline.points[i];
        Vector3 previous, next;
        if (i == 0) {
            previous = current;
        }else {
            previous = spline.points[i-1];
        }
        if (i == spline.points.size() -1) {
            next = current;
        }else {
            next = spline.points[i+1];
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


void RidgeNode::doTransformSurface(QVector < QVector < Vector3 > > & rows) {
    QVector<Vector3> pointsOnRidge;
    QVector<float> heightsOfRidge;
    for (float i=0.0; i < 1.01; i+=0.01) {
        Vector3 basePoint = baseSpline.getPoint(i);
        Vector3 topPoint = spline.getPoint(i);
        float height = fabs(topPoint.y()-basePoint.y());
        pointsOnRidge.push_back(topPoint);
        heightsOfRidge.push_back(height);
    }

    for(int j = 0 ;j <rows.size();++j) {
        for (int k = 0; k< rows[0].size();++k) {
            Vector3 & point = rows[j][k];
            Vector3 nearestTop;
            float distance = 999999999;
            float height = 0;
            for(int l = 0; l< pointsOnRidge.size();++l) {
                Vector3& pointOnRidge = pointsOnRidge[l];
                Vector3 onLevel(pointOnRidge.x(),point.y(), pointOnRidge.z());
                float localDist = (onLevel - point).lenght();
                if (localDist < distance) {
                    nearestTop = pointOnRidge;
                    distance = localDist;
                    height = heightsOfRidge[l];
                }
            }
            float radiusOfInfluence = height;
            if (distance <= radiusOfInfluence) {
                float multiplicator = (radiusOfInfluence-distance)/radiusOfInfluence;
                point = Vector3(point.x(), point.y()+height*multiplicator, point.z());
            }
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
