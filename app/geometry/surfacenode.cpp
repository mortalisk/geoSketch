#include "surfacenode.h"
#include "surface.h"
#include "ridgenode.h"
#include "util.h"

SurfaceNode::SurfaceNode(QString name , Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below) : BaseNode(name), front(front), right(right), back(back), left(left), below(below), hasContructedLayer(false)
{
    //constructLayer();
}

SurfaceNode::SurfaceNode(SurfaceNode &other): BaseNode(other) ,front(other.front), right(other.right), back(other.back), left(other.left), below(other.below), hasContructedLayer(false){

}

BaseNode* SurfaceNode::copy() {
    return new SurfaceNode(*this);
}

void SurfaceNode::prepareForDrawing()  {
    constructLayer();
}

void SurfaceNode::invalidate() {
    hasContructedLayer = false;
    if (shape != NULL)
        delete shape;
    shape = NULL;
}

void SurfaceNode::makeSide(Spline& belowSpline, Spline& spline,QVector<Vector3>& normals, QVector<Vector3>& triangles) {

    QVector<Vector3> front1, front2;
    for(float i = 0.0; i < 1.0; i+= 0.02) {
        Vector3 a = belowSpline.getPoint(i);
        Vector3 b = belowSpline.getPoint(i+0.02);

        Vector3 c = spline.getPoint(i);
        Vector3 d = spline.getPoint(i+0.02);

        front1.push_back(a);
        front2.push_back(c);

    }
    for (int i = front2.size()-1; i>= 0; --i) {
        front1.push_back(front2[i]);
    }

    front2.clear();
    Axis axis = X;
    bool similarZ = similar(front1[0].z(),front1[front1.size()-1].z(),front1[2].z());
    if (similarZ) {
        axis = Z;
    }

    triangulate(front1,front2, axis);


    for (int i = 0; i < front2.size(); i+=3) {
        Vector3 a = front2[i];
        Vector3 b = front2[i+1];
        Vector3 c = front2[i+2];
        Vector3 normal = (b-a).cross(c-a).normalize();

        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
        triangles.push_back(a);
        triangles.push_back(b);
        triangles.push_back(c);
    }

}

void SurfaceNode::constructLayer() {
    if (hasContructedLayer) return;

    QVector<Vector3> triangles;
    QVector4D c(0.1, 0.3, 0.4, 1.0);
    QVector<Vector3> previousRow;


    Vector3 frontRight = right.getPoint(0.0);
    Vector3 frontLeft = left.getPoint(1.0);
    Vector3 backLeft = left.getPoint(0.0);
    Vector3 backRight = right.getPoint(1.0);

    QVector < QVector < Vector3 > > rows;
    for (double zi = 0.0;zi<=1.01;zi+=0.02) {
        QVector<Vector3> row;
        Vector3 rowLeft = frontLeft*(1.0-zi) + backLeft *(zi);
        Vector3 rowRigth = frontRight*(1.0-zi) + backRight * (zi);

        Vector3 leftp = left.getPoint(1.0-zi);
        Vector3 rightp = right.getPoint(zi);
        for (double xi = 0.0;xi<=1.01;xi+=0.02) {
            Vector3 colInt = rowLeft * (1.0-xi) + rowRigth * xi;
            Vector3 frontp = front.getPoint(xi);
            Vector3 backp = back.getPoint(1.0-xi);
            Vector3 frontBack = frontp*(1.0-zi)+backp*zi;
            Vector3 diff = frontBack - colInt;

            Vector3 leftRight = leftp*(1.0-xi) + rightp*xi;
            Vector3 point = leftRight + diff;
            row.push_back(point);
        }
        rows.push_back(row);
    }

    foreach (BaseNode * child , children) {
        ISurfaceFeature * feature = dynamic_cast<ISurfaceFeature *>(child);
        if (feature != NULL)
            feature->doTransformSurface(rows);
    }

    //compute normals
    QVector < QVector < Vector3 > > normalRows;
    for (int i = 0; i< rows.size(); ++i) {
        QVector<Vector3> row;
        for (int j = 0; j < rows[i].size(); ++j) {
            Vector3 a,b,c,d;
            if (i == 0) {
                a = Vector3(rows[i+1][j] - rows[i][j]);
                a = -a;
            }else {
                a = Vector3(rows[i-1][j] - rows[i][j]);
            }
            if (i == rows.size()-1) {
                b = Vector3(rows[i-1][j] - rows[i][j]);
                b = -b;
            }else {
                b = Vector3(rows[i+1][j] - rows[i][j]);
            }
            if (j == 0) {
                c = Vector3(rows[i][j+1] - rows[i][j]);
                c = -c;
            }else {
                c = Vector3(rows[i][j-1] - rows[i][j]);
            }
            if (j == rows[i].size()-1) {
                d = Vector3(rows[i][j-1] - rows[i][j]);
                d = -d;
            }else {
                d = Vector3(rows[i][j+1] - rows[i][j]);
            }
            Vector3 n = (a.cross(d) + d.cross(b) + b.cross(c) + c.cross(a)).normalize();
            row.push_back( n);
        }
        normalRows.push_back(row);
    }

    QVector<Vector3> normals;
    //create triangles
    for (int i = 1; i< rows.size(); ++i) {
        for (int j = 1; j < rows[i].size(); ++j) {
            Vector3 & a = rows[i-1][j-1];
            Vector3 & na = normalRows[i-1][j-1];
            Vector3 & b = rows[i-1][j];
            Vector3 & nb = normalRows[i-1][j];
            Vector3 & c = rows[i][j-1];
            Vector3 & nc = normalRows[i][j-1];
            Vector3 & d = rows[i][j];
            Vector3 & nd = normalRows[i][j];
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            triangles.push_back(b);
            triangles.push_back(d);
            triangles.push_back(c);
            normals.push_back(na);
            normals.push_back(nb);
            normals.push_back(nc);
            normals.push_back(nb);
            normals.push_back(nd);
            normals.push_back(nc);

        }
    }

    QVector<Vector3> outline;

    for (double i = 0.0;i<=1.01;i+=0.02) {
        outline.push_back(front.getPoint(i));
    }
    for (double i = 0.0;i<=1.01;i+=0.02) {
        outline.push_back(right.getPoint(i));
    }
    for (double i = 0.0;i<=1.01;i+=0.02) {
        outline.push_back(back.getPoint(i));
    }
    for (double i = 0.0;i<=1.01;i+=0.02) {
        outline.push_back(left.getPoint(i));
    }

    if (below) {
        makeSide(below->front, front, normals, triangles);
        makeSide(below->left, left, normals, triangles);
        makeSide(below->back, back, normals, triangles);
        makeSide(below->right, right, normals, triangles);
    }


    hasContructedLayer = true;
    shape = new Surface(triangles, normals, outline);
}

QVector<Vector3> SurfaceNode::triangulatePolygon(QVector<Vector3> vertices) {

}

void SurfaceNode::determineActionOnStoppedDrawing() {
    BaseNode::determineActionOnStoppedDrawing();

    makeRidgeNode();
}

void SurfaceNode::makeRidgeNode() {

    if (spline.points.size() < 2)
        return;
    RidgeNode * ridge = new RidgeNode(spline);
    ridge->parent = this;
    children.append(ridge);

    spline.points.clear();

    ridge->makeWall();
    proxy = ridge;

    hasContructedLayer = false;
    delete shape;
    shape = NULL;


}

void SurfaceNode::drawChildren() {
//    if (active) {
        BaseNode::drawChildren();
//    }
}
