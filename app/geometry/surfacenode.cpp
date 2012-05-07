 #include "surfacenode.h"
#include "surface.h"
#include "ridgenode.h"
#include "rivernode.h"
#include "util.h"

SurfaceNode::SurfaceNode(QString name , Spline& front, Spline& right, Spline& back, Spline& left, SurfaceNode * below) : BaseNode(name), front(front), right(right), back(back), left(left), below(below), hasContructedLayer(false),resolution(200), skip(2)
{
    //constructLayer();
}

SurfaceNode::SurfaceNode(SurfaceNode &other): BaseNode(other) ,front(other.front), right(other.right), back(other.back), left(other.left), below(other.below), hasContructedLayer(false), resolution(other.resolution), skip(other.skip), uvCoordinateSpline(other.uvCoordinateSpline){

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

void SurfaceNode::makeSide(Spline& belowSpline, Spline& spline, QVector<vertex>& triangles, QVector<Vector3>& boxoutline) {
    Vector3 splinep1 = spline.getPoints()[0];
    Vector3 splinep2 = spline.getPoints()[2];
    Vector3 belowp = belowSpline.getPoints()[0];
    Axis axis = X;
    bool similarZ = similar(splinep1.z(),belowp.z(),splinep2.z());
    if (similarZ) {
        axis = Z;
    }

    QVector<Vector3> front1, front2, outline;
    for(float i = 0.0; i <= 1.001; i+= 0.02) {


        Vector3 a = spline.getPoint(i);
        Vector3 b = belowSpline.getPoint(i);


        front1.push_back(a);
        front2.push_back(b);

    }

    bool add = splinep1.y()>belowp.y();
    int indexInBelowOfIntersection = -1;

    for (int i = 0; i <front1.size()-1; ++i) {
        Vector3& a =front1[i];
        Vector3& b =front1[i+1];
        if (add) {
            outline.push_back(a);
        }

        for (int j = 0; j <front2.size()-1; ++j) {
            Vector3& c =front2[j];
            Vector3& d =front2[j+1];

            double x,y;
            if (axis == X && lineSegmentIntersection(a.z(),a.y(), b.z(), b.y(), c.z(), c.y(), d.z(), d.y(), &x, &y)) {
                outline.push_back(Vector3(a.x(),y,x));
                add = !add;
                indexInBelowOfIntersection = j;
            }else if (axis == Z && lineSegmentIntersection(a.x(),a.y(), b.x(), b.y(), c.x(), c.y(), d.x(), d.y(), &x, &y)) {
                outline.push_back(Vector3(x,y,a.z()));
                add = !add;
                indexInBelowOfIntersection = j;
            }

        }
    }
    if (add) {
        outline.push_back(front1[front1.size()-1]);
    }

    if (add) {
        for (int i = front2.size()-1; i> indexInBelowOfIntersection; --i) {
            outline.push_back(front2[i]);
        }
    }else {
        for (int i = indexInBelowOfIntersection; i>= 0; --i) {
            outline.push_back(front2[i]);
        }
    }

    front2.clear();


    triangulate(outline,front2, axis);


    for (int i = 0; i < front2.size(); i+=3) {
        Vector3 a = front2[i];
        Vector3 b = front2[i+1];
        Vector3 c = front2[i+2];
        Vector3 normal = (b-a).cross(c-a).normalize();

        triangles.push_back(vertex(a,normal));
        triangles.push_back(vertex(b,normal));
        triangles.push_back(vertex(c,normal));
    }

    for (double i = 0.0;i<=1.01;i+=0.02) {
        boxoutline.push_back(spline.getPoint(i));
    }

}

void SurfaceNode::constructLayer() {
    if (hasContructedLayer) return;

    QVector<vertex> triangles;
    QVector4D c(0.1, 0.3, 0.4, 1.0);
    QVector<Vector3> previousRow;


    Vector3 frontRight = right.getPoint(0.0);
    Vector3 frontLeft = left.getPoint(1.0);
    Vector3 backLeft = left.getPoint(0.0);
    Vector3 backRight = right.getPoint(1.0);

    rows.clear();
    intersectRows.clear();

    for (int zi = 0;zi<=resolution;++zi) {
        QVector<Vector3> row;
        float zif = zi/(float)resolution;
        Vector3 rowLeft = frontLeft*(1.0-zif) + backLeft *(zif);
        Vector3 rowRigth = frontRight*(1.0-zif) + backRight * (zif);

        Vector3 leftp = left.getPoint(1.0-zif);
        Vector3 rightp = right.getPoint(zif);
        for (int xi = 0;xi<=resolution;xi++) {
            float xif = xi/(float)resolution;
            Vector3 colInt = rowLeft * (1.0-xif) + rowRigth * xif;
            Vector3 frontp = front.getPoint(xif);
            Vector3 backp = back.getPoint(1.0-xif);
            Vector3 frontBack = frontp*(1.0-zif)+backp*zif;
            Vector3 diff = frontBack - colInt;

            Vector3 leftRight = leftp*(1.0-xif) + rightp*xif;
            Vector3 point = leftRight + diff;
            row.push_back(point);
        }
        rows.push_back(row);
    }

//    for (int z = 0; z<resolution; z+=10) {
//        QVector<Vector3> row;
//        for (int x = 0; x < resolution; z+=10) {
//            row.push_back(rows[z][x]);
//        }
//        intersectRows.push_back(row);
//    }

    foreach (BaseNode * child , children) {
        ISurfaceFeature * feature = dynamic_cast<ISurfaceFeature *>(child);
        if (feature != NULL) {
            feature->repositionOnSurface(*this);
            feature->doTransformSurface(rows, resolution, 10);
        }
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
            triangles.push_back(vertex(a,na));
            triangles.push_back(vertex(b,nb));
            triangles.push_back(vertex(c,nc));
            triangles.push_back(vertex(b,nb));
            triangles.push_back(vertex(d,nd));
            triangles.push_back(vertex(c,nc));

        }
    }

    QVector<Vector3> outline;

    if (below) {
        makeSide(below->front, front, triangles, outline);
        makeSide(below->right, right, triangles, outline);
        makeSide(below->back, back, triangles, outline);
        makeSide(below->left, left, triangles, outline);
    }


    hasContructedLayer = true;
    shape = new Surface(triangles, outline);
}

void SurfaceNode::determineActionOnStoppedDrawing() {
    BaseNode::determineActionOnStoppedDrawing();

    //makeRidgeNode();
}

void SurfaceNode::makeRiverNode() {

    if (spline.getPoints().size() < 2)
        return;
    RiverNode * ridge = new RiverNode(uvCoordinateSpline, this);
    ridge->parent = this;
    children.append(ridge);

    spline.clear();
    sketchingSpline.clear();
    uvCoordinateSpline.clear();

    ridge->makeWater();
    proxy = ridge;

    hasContructedLayer = false;
    delete shape;
    shape = NULL;


}

void SurfaceNode::makeRidgeNode() {

    if (spline.getPoints().size() < 2)
        return;
    RidgeNode * ridge = new RidgeNode(uvCoordinateSpline, this);
    ridge->parent = this;
    children.append(ridge);

    spline.clear();
    sketchingSpline.clear();
    uvCoordinateSpline.clear();

    ridge->makeWall();
    proxy = ridge;

    hasContructedLayer = false;
    delete shape;
    shape = NULL;


}

int intersect(Vector3& p, Vector3& dir, Vector3& v0, Vector3& v1, Vector3& v2, Vector3* I, float* sp, float* tp) {
    // hentet fra http://softsurfer.com/Archive/algorithm_0105/algorithm_0105.htm#intersect_RayTriangle%28%29

    Vector3    u, v, n;             // triangle vectors
    Vector3    w0, w;          // ray vectors
    float      r, a, b;             // params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = v1 - v0;
    v = v2 - v0;
    n = u.cross(v);             // cross product
    if (n.x() == 0 && n.y()== 0 && n.z() == 0)            // triangle is degenerate
        return -1;               // do not deal with this case

    //dir = R.P1 - R.P0;             // ray direction vector
    w0 = p - v0;
    a = -(n * w0);
    b = n * dir;
    if (fabs(b) < 0.01) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    *I = p + r *dir;           // intersect point of ray and plane

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = u*u;
    uv = u*v;
    vv = v*v;
    w = *I - v0;
    wu = w*u;
    wv = w*v;
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    *sp = s;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    *tp = t;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;

    return 1;                      // I is in T
}

QVector<Vector3> SurfaceNode::intersectionPoints(Vector3 from,Vector3 direction) {
    QVector<Vector3> cand;
    for (int i = 0; i < rows.size()-skip; i+=skip) {
        for (int j = 0; j < rows[0].size()-skip; j+=skip) {
            Vector3& a = rows[i][j];
            Vector3& b = rows[i][j+skip];
            Vector3& c = rows[i+skip][j];
            Vector3& d = rows[i+skip][j+skip];

            Vector3 result;
            int r;
            float s,t;
            r = intersect(from, direction, a, b, c,&result, &s, &t);
            if (r==1) {
                cand.push_back(result);
            }

            r = intersect(from, direction, d, c, b,&result, &s, &t);
            if (r==1) {
                cand.push_back(result);
            }

        }
    }
    float nearestDist = 10000000;
    int nearest = -1;
    for (int i = 0; i< cand.size(); ++i) {
        float dist = (cand[i]-from).lenght();
        if (dist < nearestDist) {
            nearest = i;
            nearestDist = dist;
        }
    }
    if (cand.size() >1) {
        Vector3 tmp = cand[0];
        cand[0] = cand[nearest];
        cand[nearest] = tmp;
    }
    return cand;
}

void SurfaceNode::addPoint(Vector3 from, Vector3 direction) {
    QVector<Vector3> cand;
    QVector<QVector2D> uvCand;
    float resolution = 1.0/(float)this->resolution;
    for (int i = 0; i < rows.size()-skip; i+=skip) {
        for (int j = 0; j < rows[0].size()-skip; j+=skip) {
            Vector3& a = rows[i][j];
            Vector3& b = rows[i][j+skip];
            Vector3& c = rows[i+skip][j];
            Vector3& d = rows[i+skip][j+skip];

            Vector3 result;
            int r;
            float s,t;
            r = intersect(from, direction, a, b, c,&result, &s, &t);
            if (r==1) {
                cand.push_back(result);
                s*=resolution;
                t*=resolution;
                uvCand.push_back(QVector2D(j*resolution+s*skip, i*resolution+t*skip));
            }

            r = intersect(from, direction, d, c, b,&result, &s, &t);
            if (r==1) {
                cand.push_back(result);
                s = 1.0-s;
                t = 1.0-t;
                s*=resolution;
                t*=resolution;
                uvCand.push_back(QVector2D(j*resolution+s*skip, i*resolution+t*skip));
            }

        }
    }
    float nearestDist = 10000000;
    int nearest = -1;
    for (int i = 0; i< cand.size(); ++i) {
        float dist = (cand[i]-from).lenght();
        if (dist < nearestDist) {
            nearest = i;
            nearestDist = dist;
        }
    }

    if (nearest != -1) {
        sketchingSpline.addPoint(cand[nearest]);
        uvCoordinateSpline.push_back(uvCand[nearest]);
    }
}

Vector3 SurfaceNode::getPointFromUv(QVector2D uv) {
//    Vector3 frontRight = right.getPoint(0.0);
//    Vector3 frontLeft = left.getPoint(1.0);
//    Vector3 backLeft = left.getPoint(0.0);
//    Vector3 backRight = right.getPoint(1.0);

//    float xi = uv.x();
//    float zi = uv.y();


//    Vector3 rowLeft = frontLeft*(1.0-zi) + backLeft *(zi);
//    Vector3 rowRigth = frontRight*(1.0-zi) + backRight * (zi);

//    Vector3 leftp = left.getPoint(1.0-zi);
//    Vector3 rightp = right.getPoint(zi);

//    Vector3 colInt = rowLeft * (1.0-xi) + rowRigth * xi;
//    Vector3 frontp = front.getPoint(xi);
//    Vector3 backp = back.getPoint(1.0-xi);
//    Vector3 frontBack = frontp*(1.0-zi)+backp*zi;
//    Vector3 diff = frontBack - colInt;

//    Vector3 leftRight = leftp*(1.0-xi) + rightp*xi;
//    Vector3 point = leftRight + diff;
    float x = uv.x()*(resolution-1);
    float z = uv.y()*(resolution-1);
    int xi = floor(x);
    int zi = floor(z);
    float u = (x - xi);
    float v = (z - zi);
    int nextx = u > 0.0001? 1: 0;
    int nextz = v > 0.0001? 1: 0;
    Vector3& a = rows[zi][xi];
    Vector3& b = rows[zi][xi+nextx];
    Vector3& c = rows[zi+nextz][xi];
    Vector3& d = rows[zi+nextz][xi+nextx];

    Vector3 ab = a*(1-u) + b*u;
    Vector3 cd = c*(1-u) + d*u;

    Vector3 point = ab * (1-v) + cd * v;

    return point;
}

void SurfaceNode::drawChildren() {
//    if (active) {
        BaseNode::drawChildren();
//    }
}
