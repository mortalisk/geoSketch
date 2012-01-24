#include "surfacenode.h"
#include "surface.h"

SurfaceNode::SurfaceNode(QString name , Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below) : Node(name), front(front), right(right), back(back), left(left), below(below)
{
    constructLayer();
}

void SurfaceNode::constructLayer() {

    QVector<Vector3> triangles;
    QVector4D c(0.1, 0.3, 0.4, 1.0);
    QVector<Vector3> previousRow;


    Vector3 frontRight = right.getPoint(0.0);
    Vector3 frontLeft = left.getPoint(1.0);
    Vector3 backLeft = left.getPoint(0.0);
    Vector3 backRight = right.getPoint(1.0);

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
        if (previousRow.size() == row.size()) {
            for (int i = 0; i < previousRow.size()-1; ++i) {
                Vector3 & a = previousRow[i];
                Vector3 & b = previousRow[i+1];
                Vector3 & c = row[i];
                Vector3 & d = row[i+1];
                triangles.push_back(a);
                triangles.push_back(b);
                triangles.push_back(c);
                triangles.push_back(b);
                triangles.push_back(d);
                triangles.push_back(c);
            }
        }
        previousRow = row;
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
        for(float i = 0.0; i < 1.0; i+= 0.02) {
            Vector3 a = below->front.getPoint(i);
            Vector3 b = below->front.getPoint(i+0.02);

            Vector3 c = front.getPoint(i);
            Vector3 d = front.getPoint(i+0.02);
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            triangles.push_back(c);
            triangles.push_back(b);
            triangles.push_back(d);
        }
        for(float i = 0.0; i < 1.0; i+= 0.02) {
            Vector3 a = below->right.getPoint(i);
            Vector3 b = below->right.getPoint(i+0.02);

            Vector3 c = right.getPoint(i);
            Vector3 d = right.getPoint(i+0.02);
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            triangles.push_back(c);
            triangles.push_back(b);
            triangles.push_back(d);
        }
        for(float i = 0.0; i < 1.0; i+= 0.02) {
            Vector3 a = below->back.getPoint(i);
            Vector3 b = below->back.getPoint(i+0.02);

            Vector3 c = back.getPoint(i);
            Vector3 d = back.getPoint(i+0.02);
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            triangles.push_back(c);
            triangles.push_back(b);
            triangles.push_back(d);
        }
        for(float i = 0.0; i < 1.0; i+= 0.02) {
            Vector3 a = below->left.getPoint(i);
            Vector3 b = below->left.getPoint(i+0.02);

            Vector3 c = left.getPoint(i);
            Vector3 d = left.getPoint(i+0.02);
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            triangles.push_back(c);
            triangles.push_back(b);
            triangles.push_back(d);
        }
    }

    shape = new Surface(triangles, outline);
}
