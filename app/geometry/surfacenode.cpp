#include "surfacenode.h"
#include "surface.h"
#include <GL/glew.h>
#include "ridgenode.h"

SurfaceNode::SurfaceNode(QString name , Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below) : BaseNode(name), front(front), right(right), back(back), left(left), below(below), hasContructedLayer(false)
{
    //constructLayer();
}

SurfaceNode::SurfaceNode(SurfaceNode &other): BaseNode(other) ,front(other.front), right(other.right), back(other.back), left(other.left), below(other.below), hasContructedLayer(false){

}

BaseNode* SurfaceNode::copy() {
    return new SurfaceNode(*this);
}

void SurfaceNode::drawWall() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBegin(GL_TRIANGLE_STRIP);
    float color[4] = {0.7,0.4,0.4,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);

    float spec[4] = {1.0,1.0,1.0,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
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
        Vector3 up(0, 1, 0);
        Vector3 normal = (next-previous).cross(up).normalize();
        glNormal3f(normal.x(),normal.y(),normal.z());
        glVertex3d(current.x(),current.y(),current.z());
        glVertex3d(current.x(),current.y()+5,current.z());


    }
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
        Vector3 up(0, 1, 0);
        Vector3 normal = (next-previous).cross(up).normalize();
        glNormal3f(normal.x(),normal.y(),normal.z());
        glVertex3d(current.x(),current.y(),current.z());
        glVertex3d(current.x(),current.y()+5,current.z());


    }
    glEnd();
    glDisable(GL_CULL_FACE);
}

void SurfaceNode::drawSelf() {

    if (spline.points.size() > 1) {
        drawWall();
    }
    BaseNode::drawSelf();
}

void SurfaceNode::prepareForDrawing()  {
    constructLayer();
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
        for(float i = 0.0; i < 1.0; i+= 0.02) {
            Vector3 a = below->front.getPoint(i);
            Vector3 b = below->front.getPoint(i+0.02);

            Vector3 c = front.getPoint(i);
            Vector3 d = front.getPoint(i+0.02);
            Vector3 normal = (b-a).cross(c-a).normalize();
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
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
            Vector3 normal = (b-a).cross(c-a).normalize();
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
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
            Vector3 normal = (b-a).cross(c-a).normalize();
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
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
            Vector3 normal = (b-a).cross(c-a).normalize();
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            triangles.push_back(c);
            triangles.push_back(b);
            triangles.push_back(d);
        }
    }
    hasContructedLayer = true;
    shape = new Surface(triangles, normals, outline);
}

void SurfaceNode::determineActionOnStoppedDrawing() {
    BaseNode::determineActionOnStoppedDrawing();

    makeRidgeNode();
}

void SurfaceNode::makeRidgeNode() {

    RidgeNode * ridge = new RidgeNode(spline);
    ridge->parent = this;
    children.append(ridge);

    spline.points.clear();


}
