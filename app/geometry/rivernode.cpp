#include "util.h"
#include "rivernode.h"
#include "surfacenode.h"
#include "surface.h"
#include "deposit.h"
#include <float.h>
#include <QVector2D>
#include <QAction>
#include <QMenu>
#include <GL/glew.h>
RiverNode::RiverNode(QVector<QVector2D> uvs, bool drawWater, float width) : BaseNode("river"), uv(uvs), deposit(NULL), drawWater(drawWater)
{
    deposit = NULL;
    smooth(uv);
    QVector2D prev = uv[0] - (uv[1]- uv[0]);
    for (int i = 0; i<uv.size(); ++i) {
        QVector2D next;
        if (i == uv.size() -1) {
            next = uv[i] + (uv[i]-uv[i-1]);
        }else {
            next = uv[i+1];
        }
        QVector2D normal = next - prev;
        normal = QVector2D(normal.y(), -normal.x());
        normal.normalize();

        //float distanceFromMiddle = fabs(i-uv.size()/2.0)/uv.size();

        float w = width;
        float startl = (uv[i]-uv[0]).length();
        float endl = (uv[i]-uv[uv.size()-1]).length();
        if (startl <= 0.05)
            w *= fmax(0,log10(startl/0.05 *100))/2;
        else if (endl <= 0.05)
            w *= fmax(0,log10(endl/0.05 *100))/2;

        normal *= w;

        lefts.push_back(uv[i] + normal);
        rigths.push_back(uv[i] - normal);
        prev = uv[i];
    }
}

float RiverNode::dist(float s, float t) {
    QVector2D p(s,t);
    float distance = FLT_MAX;
    for (int i = 0; i<lefts.size(); i++) {
        float dist = (lefts[i] - p).length();
        if (dist < distance) {
            distance = dist;
            editLeft = true;

        }
    }
    for (int i = 0; i<rigths.size(); i++) {
        float dist = (rigths[i] - p).length();
        if (dist < distance) {
            distance = dist;
            editRight = true;
            editLeft = false;
        }
    }
    return distance;
}

QVector<Vector3> RiverNode::intersectionPoints(Vector3 from, Vector3 direction) {
//    if (active) {
//        return BaseNode::intersectionPoints(from, direction);
//    }else {
//        QVector<Vector3> intersects = parent->intersectionPoints(from, direction);
//        if (intersects.size() > 0) {
//            for (float i = 0.0; i < 1.01;i+=0.1) {
//                Vector3 p = spline.getPoint(i);
//                if ((intersects[0] - p).lenght() < 1.0) {
//                    return intersects;
//                }
//            }

//            intersects.clear();
//        }
//        return intersects;
//    }

        lastIntersectionFrom = from;
        lastIntersectionDriection = direction;
//    if (drawWater) {
//        return BaseNode::intersectionPoints(from, direction);
//    }
//    else {
        float s, t;
        QVector<Vector3> pointsParent = SurfaceNode::intersectionOnRows(from, direction, rows, s, t, skip);
        for(int i = 0; i<lefts.size(); i++) {
            QVector2D p(s,t);
            if ((lefts[i]-p).length() < 0.03 || (rigths[i]-p).length() < 0.03) {
                return pointsParent;
            }
        }
        return QVector<Vector3>();

//    }

}

void RiverNode::determineActionOnStoppedDrawing() {
    //BaseNode::determineActionOnStoppedDrawing();

    doOversketch();


    SurfaceNode * surfaceParent = dynamic_cast<SurfaceNode *>(parent);
    surfaceParent->invalidate();

}

void RiverNode::addPoint(Vector3 from, Vector3 direction) {
    //BaseNode::addPoint(from, direction);
    float s, t;

    SurfaceNode * surfaceParent = dynamic_cast<SurfaceNode *>(parent);

    //float size = 10.0f;
    QVector<Vector3> pointsParent = SurfaceNode::intersectionOnRows(from, direction, rows, s, t, skip);
    //QVector<Vector3> pointsThis = shape->intersectionPoints(from, direction, s2, t2);

//    if (pointsThis.size() > 0) {
//        pointsParent = pointsThis;
//        s = s2;
//        t = t2;
//    }
    Vector3 point;
    QVector2D point2d;

    if (pointsParent.size() > 0) {
        point = pointsParent[0];
        point2d.setX(s);
        point2d.setY(t);


        sketchingSpline.addPoint(point/*Vector3(point.x(), point.y()+0.01, point.z())*/);
        uvSketch.push_back(point2d);
    } else {
        return;
    }

}

void RiverNode::doOversketch() {

    if (lefts.size() <2 || rigths.size() < 2 ) return;


    int leftFirst = -1, leftLast = -1;
    nearest(uvSketch[0], uvSketch[uvSketch.size()-1],lefts, leftFirst, leftLast);

    int rigthFirst = -1, rigthLast = -1;
    nearest(uvSketch[0], uvSketch[uvSketch.size()-1],rigths, rigthFirst, rigthLast);

    if (editLeft) {
        // oversketching the left side
        insertInto(lefts, uvSketch, leftFirst, leftLast, !oversketch);
        smooth(lefts);

    } else {
        // oversketching the rigth side
        insertInto(rigths, uvSketch, rigthFirst, rigthLast, !oversketch);
        smooth(rigths);
    }

    uvSketch.clear();
    sketchingSpline.clear();

}

void RiverNode::makeWater() {
    this->diffuse = QVector4D(0.0,0.0,0.8,0.5);
    QVector<vertex> triangles;

    Spline& spline = this->spline;

    for (int i = 0 ; i < spline.getPoints().size(); ++i) {
        Vector3 current = spline.getPoints()[i];
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
        Vector3 left = rightSpline.getPoints()[i]-current;
        Vector3 normal = (next-previous).cross(left).normalize();
        if (normal.y() < 0) normal = -normal;
        Vector3 otherSide = current+left;
        QVector2D uvRight;
        QVector2D uvLeft;
        current = Vector3(current.x(), current.y()+0.005, current.z());
        otherSide = Vector3(otherSide.x(), otherSide.y()+0.005, otherSide.z());
        triangles.push_back(vertex(current,normal));
        triangles.push_back(vertex(otherSide,normal));


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
    Surface* s = new Surface(triangles, noOutline,true);
    shape = s;
}

void RiverNode::setActive(bool a) {
    oversketch = true;
    BaseNode::setActive(a);

    if (a) {

        float s, t;

        //float size = 10.0f;
        QVector<Vector3> pointsParent = SurfaceNode::intersectionOnRows(lastIntersectionFrom, lastIntersectionDriection, rows, s, t, skip);
        QVector2D lastIntersection(s, t);
        float nearestLeft = FLT_MAX;
        for (int i = 0; i<lefts.size(); i++) {
            float l = (lefts[i]-lastIntersection).length();
            if (l< nearestLeft)
                nearestLeft = l;
        }

        float nearestRight = FLT_MAX;
        for (int i = 0; i<rigths.size(); i++) {

            float l = (rigths[i]-lastIntersection).length();
            if (l< nearestRight)
                nearestRight = l;
        }

//        editLeft = nearestLeft < nearestRight;
//        editRight = !editLeft;
    } else {
        editLeft = false;
        editRight = false;
    }
}

void RiverNode::addSubclassActions(QToolBar *menu) {

    QAction * replace = new QAction(QString("Replace side"), menu);
    replace->setCheckable(true);
    replace->setChecked(!oversketch);
    connect(replace, SIGNAL(toggled(bool)), this, SLOT(replace(bool)));
    menu->addAction(replace);
}

void RiverNode::doTransformSurface(QVector < QVector < Vector3 > > & rows, float resolution, float) {
    if (rightSpline.getPoints().size() < 2 || spline.getPoints().size() < 2) return;

    float cellsize = 1.0/resolution;
    //float worldToGrid = resolution/size;

    //QVector < float > fill(rows[0].size());
    //QVector < QVector < float > > heigths(rows.size(), fill);
    //smooth(lefts);
    //smooth(rigths);

    for (int i = 0; i< lefts.size()-1;++i) {

        QVector2D a = lefts[i]/cellsize;
        QVector2D b = lefts[i+1]/cellsize;
        QVector2D c = rigths[i]/cellsize;
        QVector2D d = rigths[i+1]/cellsize;
        float zmin = std::min(std::min(a.y(), b.y()), std::min(c.y(), d.y()));
        float zmax = std::max(std::max(a.y(), b.y()), std::max(c.y(), d.y()));
        float xmin = std::min(std::min(a.x(), b.x()), std::min(c.x(), d.x()));
        float xmax = std::max(std::max(a.x(), b.x()), std::max(c.x(), d.x()));
//        zmin = fmax (zmin, 0);
//        zmax = fmin (zmax, resolution);
//        xmin = fmax (xmin, 0);
//        xmax = fmin (xmax, resolution);
//        for (int z = 0; z<rows.size(); ++z) {
//            for (int x = 0; x<rows[0].size();++x) {
        for (int z =zmin; z<=zmax; ++z) {
            for (int x = xmin; x<xmax;++x) {
                QVector2D point(x,z);

                float u, v;
                if (inside(a,b,c,point, &u, &v)
                        || inside(c,b,d,point, &u, &v)) {

//                    QVector2D CA = a-c;
//                    QVector2D DB = b-d;

//                    QVector2D inter = (CA + DB).normalized();

                    float distAB = minimum_distance(a, b, point);
                    float distCD = minimum_distance(c, d, point);
//                    float distCA = distToLine(c, a, point);
//                    float distDB = distToLine(d, b, point);
//                    float ac = (a-c).length();
//                    float bd = (b-d).length();
//                    float w0 = distCA/(distCA+distDB);
//                    float w1 = distDB/(distCA+distDB);

//                    float depth0 = 0.2*distCA;
//                    float depth1 = 0.2*distDB;
//                    float depth =

                    float maxDepth = -0.2;
                    float depth = depthOfRiver(fmin(distAB, distCD), 0.03*resolution , maxDepth);

                    if (z >=0 && z <resolution &&
                            x >=0 && x < resolution) {
                        rows[z][x] = Vector3(rows[z][x].x(),rows[z][x].y() + depth,rows[z][x].z());
                    }
                }
            }
        }
    }
//    for (int z = 0; z<rows.size(); ++z) {
//        for (int x = 0; x<rows[0].size();++x) {
//            rows[z][x] = Vector3(rows[z][x].x(),rows[z][x].y()+heigths[z][x],rows[z][x].z());
//        }
//    }


}

void RiverNode::smooth(QVector<QVector2D>& uv) {
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

void RiverNode::removeLoops(QVector<QVector2D>& sp) {
    for (int i = 0; i <sp.size()-1; ++i) {
        QVector2D& A = sp[i];
        QVector2D& B = sp[i+1];
        for (int j = i+1; j <sp.size()-1; ++j) {
            QVector2D& C = sp[j];
            QVector2D& D = sp[j+1];
            double X,Y;

            if (lineSegmentIntersection(A.x(),A.y(),B.x(), B.y(), C.x(), C.y(), D.x(), D.y(), &X, &Y)){
                for (int p = i+1; p <=j;++p) {
                    sp[p].setX(X);
                    sp[p].setY(Y);
                }
            }
        }
    }
}

void RiverNode::stopDeposit() {
    if (deposit)
        deposit->setDepositing(!deposit->isDepositing());
    this->deposit = NULL;
}

void RiverNode::repositionOnSurface(SurfaceNode &surfacenode) {
    rightSpline.clear();
    spline.clear();

    rows = surfacenode.rows;
    skip = surfacenode.skip;

    QVector<QVector2D>& rigthPoints = rigths;
    QVector<QVector2D>& leftPoints = lefts;

    removeLoops(rigthPoints);
    removeLoops(leftPoints);

    for (int i= 0; i< rigthPoints.size(); ++i) {
        rightSpline.addPoint(surfacenode.getPointFromUv(rigthPoints[i]));
    }

    for (int i= 0; i< leftPoints.size(); ++i) {
        spline.addPoint(surfacenode.getPointFromUv(leftPoints[i]));
    }

    if (shape != NULL) {
        delete shape;
        shape = NULL;

    }
    makeWater();
}

void RiverNode::drawSelf() {
    if (drawWater)
        BaseNode::drawSelf();
    else if (active)
        drawSplines();
}

void RiverNode::drawSplines() {
    float r = active?1:0;
    if (drawWater || active) {
        drawSpline(spline,active && editLeft);
        drawSpline(rightSpline,active && editRight);

        glDisable(GL_STENCIL_TEST);
        drawSpline(sketchingSpline,r);

        glEnable(GL_STENCIL_TEST);
    }
}

void RiverNode::addSubclassJson(QVariantMap &map) {
    QVariantList rightList = Vector2DListToVariantList(rigths);
    QVariantList leftList = Vector2DListToVariantList(lefts);

    map["rights"] = rightList;
    map["lefts"] = leftList;
    map["drawWater"] = drawWater;

}

void RiverNode::fromJsonSubclass(QVariantMap map) {
    rigths = variantToVector2DVector(map["rights"]);
    lefts = variantToVector2DVector(map["lefts"]);
    drawWater = map["drawWater"].toBool();
}

void RiverNode::createDeposit(float seaLevel, SurfaceNode& surfaceNode) {

    QVector2D depositFrom;
    QVector2D depositDirection;
    QVector4D color;
    Deposit * p = NULL;
    // find previous deposit, if any
    foreach(BaseNode* b, surfaceNode.children) {
        Deposit * c = dynamic_cast<Deposit*>(b);
        if (c != NULL){
            p = c;
        }
    }

    if (this->deposit != NULL) {
        this->deposit->setDepositing(false);
        depositFrom = this->deposit->flowFrom + this->deposit->direction.normalized()*0.05;
        depositDirection = this->deposit->direction;
        color = this->deposit->diffuse;
        if (color.x() >= 0.3) {
            color.setX(color.x() - 0.3);
            color.setY(color.y() + 0.3);
        }else {
            color.setX(color.x() + 0.3);
            color.setY(color.y() - 0.3);
        }
    } else {
        color = QVector4D(0,0.5,1,1);
        for (int i = 1; i < lefts.size()-1; i++) {
            Vector3 point = surfaceNode.getPointFromUv(lefts[i]);
            if (point.y() < seaLevel) {
                //if (i == 0) return;


               depositFrom = (lefts[i]+ rigths[i])/2;
               depositDirection =  lefts[i] - lefts[i-1];

                break;

            }
        }
    }

    if (depositFrom.x() >= 0 && depositFrom.x() <= 1 && depositFrom.y() >= 0 && depositFrom.y() <= 1) {
        Deposit * deposit = new Deposit(depositFrom,depositDirection,seaLevel, &surfaceNode, p);
        deposit->diffuse = color;
        surfaceNode.children.push_back(deposit);
        //deposit->setDepositing(true);
        deposit->parent = &surfaceNode;
        this->deposit = deposit;
        //surfaceNode.invalidate();

    }



    return;
}
