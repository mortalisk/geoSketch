#include <QVector>
#include "Vector3.h"
#include <QVector2D>

#include "boxnode.h"
#include "generalnode.h"
#include "ridgenode.h"
#include "rivernode.h"
#include "sidenode.h"
#include "surfacenode.h"
#include "deposit.h"

static const float EPSILON=0.0000000001f;

BaseNode * create(QString type) {
    if (type == "BoxNode") {
        return new BoxNode;
    } else if (type == "GeneralNode") {
        return new GeneralNode;
    } else if (type == "RidgeNode") {
        return new RidgeNode;
    } else if (type == "RiverNode") {
        return new RiverNode;
    } else if (type == "SideNode") {
        return new SideNode;
    } else if (type == "SurfaceNode") {
        return new SurfaceNode;
    } else if (type == "Deposit") {
        return new Deposit;
    } else {
        return NULL;
    }
}

QVariantList Vector2DListToVariantList(QVector<QVector2D>& list) {
    QVariantList r;
    foreach(const QVector2D& i, list) {
        QVariantMap p;
        p["x"] = i.x();
        p["y"] = i.y();
        r.append(p);
    }
    return r;
}

QVariantList FloatListToVariantList(QVector<float>& list) {
    QVariantList r;
    foreach(float i, list) {
        r.append((double)i);
    }
    return r;
}

QVector<float> variantToFloatVector(QVariant& variant) {
    QVector<float> v;
    foreach(const QVariant& var,variant.toList()) {
        v.append(var.toFloat());
    }
    return v;
}

QVector<QVector2D> variantToVector2DVector(QVariant& variant) {
    QVector<QVector2D> v;
    foreach(const QVariant& var,variant.toList()) {
        v.append(QVector2D(var.toMap()["x"].toDouble(),var.toMap()["y"].toDouble()));
    }
    return v;
}

float Area(const QVector<Vector3> &contour)
{

  int n = contour.size();

  float A=0.0f;

  for(int p=n-1,q=0; q<n; p=q++)
  {
    A+= contour[p].x()*contour[q].y() - contour[q].x()*contour[p].y();
  }
  return A*0.5f;
}

   /*
     InsideTriangle decides if a point P is Inside of the triangle
     defined by A, B, C.
   */
bool  InsideTriangle(float Ax, float Ay,
                      float Bx, float By,
                      float Cx, float Cy,
                      float Px, float Py)

{
  float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
  float cCROSSap, bCROSScp, aCROSSbp;

  ax = Cx - Bx;  ay = Cy - By;
  bx = Ax - Cx;  by = Ay - Cy;
  cx = Bx - Ax;  cy = By - Ay;
  apx= Px - Ax;  apy= Py - Ay;
  bpx= Px - Bx;  bpy= Py - By;
  cpx= Px - Cx;  cpy= Py - Cy;

  aCROSSbp = ax*bpy - ay*bpx;
  cCROSSap = cx*apy - cy*apx;
  bCROSScp = bx*cpy - by*cpx;

  return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
};

bool Snip(const QVector<Vector3> &contour,int u,int v,int w,int n,int *V)
{
  int p;
  float Ax, Ay, Bx, By, Cx, Cy, Px, Py;

  Ax = contour[V[u]].x();
  Ay = contour[V[u]].y();

  Bx = contour[V[v]].x();
  By = contour[V[v]].y();

  Cx = contour[V[w]].x();
  Cy = contour[V[w]].y();

  if ( EPSILON > (((Bx-Ax)*(Cy-Ay)) - ((By-Ay)*(Cx-Ax))) ) return false;

  for (p=0;p<n;p++)
  {
    if( (p == u) || (p == v) || (p == w) ) continue;
    Px = contour[V[p]].x();
    Py = contour[V[p]].y();
    if (InsideTriangle(Ax,Ay,Bx,By,Cx,Cy,Px,Py)) return false;
  }

  return true;
}

bool Process(const QVector<Vector3> &contour, const QVector<Vector3> &contour3d,QVector<Vector3> &result)
{
  /* allocate and initialize list of Vertices in polygon */

  int n = contour.size();
  if ( n < 3 ) return false;

  int *V = new int[n];

  /* we want a counter-clockwise polygon in V */

  if ( 0.0f < Area(contour) )
    for (int v=0; v<n; v++) V[v] = v;
  else
    for(int v=0; v<n; v++) V[v] = (n-1)-v;

  int nv = n;

  /*  remove nv-2 Vertices, creating 1 triangle every time */
  int count = 2*nv;   /* error detection */

  for(int m=0, v=nv-1; nv>2; )
  {
    /* if we loop, it is probably a non-simple polygon */
    if (0 >= (count--))
    {
      //** Triangulate: ERROR - probable bad polygon!
      return false;
    }

    /* three consecutive vertices in current polygon, <u,v,w> */
    int u = v  ; if (nv <= u) u = 0;     /* previous */
    v = u+1; if (nv <= v) v = 0;     /* new v    */
    int w = v+1; if (nv <= w) w = 0;     /* next     */

    if ( Snip(contour,u,v,w,nv,V) )
    {
      int a,b,c,s,t;

      /* true names of the vertices */
      a = V[u]; b = V[v]; c = V[w];

      /* output Triangle */
      result.push_back( contour3d[a] );
      result.push_back( contour3d[b] );
      result.push_back( contour3d[c] );

      m++;

      /* remove v from remaining polygon */
      for(s=v,t=v+1;t<nv;s++,t++) V[s] = V[t]; nv--;

      /* resest error detection counter */
      count = 2*nv;
    }
  }



  delete V;

  return true;
}




//enum Axis { X, Y, Z};

bool triangulate(const QVector<Vector3> &contour,QVector<Vector3> &result, Axis axis ) {
    QVector<Vector3> P = contour;
    QVector<Vector3> P2d;

    for (int j = 0; j < P.size(); ++j) {
        if (axis == X) {
            P2d.push_back(Vector3(P[j].y(),P[j].z(),0));
        } else if (axis == Y) {
            P2d.push_back(Vector3(P[j].x(),P[j].z(),0));
        } else if (axis == Z) {
            P2d.push_back(Vector3(P[j].x(),P[j].y(),0));
        }
    }

    return Process(P2d, contour, result);

}

bool similar(float a, float b) {
    return fabs(a-b) < 0.001;
}

bool similar(float a, float b, float c) {
    return similar(a,b) && similar(b,c);
}

// compute the area of a triangle using Heron's formula
float triarea(float a, float b, float c)
{
    float s = (a + b + c)/2.0;
    return sqrt(s*(s-a)*(s-b)*(s-c));
}

// compute the distance between two 2d points
float dist(float x0, float y0, float x1, float y1)
{
    float a = x1 - x0;
    float b = y1 - y0;
    return sqrt(a*a + b*b);
}

// compute the barycentric coordinates of a 2d point inside a 2d triangle
// (x0, y0) (x1, y1) (x2, y2) are the vertices of a 2d triangle
// (vx, vy) is a point inside the 2d triangle
// u, v, w are the barycentric coordinates of (vx, vy) in the triangle
void barycent(float x0, float y0, float x1, float y1, float x2, float y2,
    float vx, float vy,
    float *u, float *v, float *w)
{
    // compute the area of the big triangle
    float a = dist(x0, y0, x1, y1);
    float b = dist(x1, y1, x2, y2);
    float c = dist(x2, y2, x0, y0);
    float totalarea = triarea(a, b, c);

    // compute the distances from the outer vertices to the inner vertex
    float length0 = dist(x0, y0, vx, vy);
    float length1 = dist(x1, y1, vx, vy);
    float length2 = dist(x2, y2, vx, vy);

    // divide the area of each small triangle by the area of the big triangle
    *u = triarea(b, length1, length2)/totalarea;
    *v = triarea(c, length0, length2)/totalarea;
    *w = triarea(a, length0, length1)/totalarea;
}

void  barycent(QVector2D p0, QVector2D p1, QVector2D p2, QVector2D p, float *u, float *v, float *w ) {
    barycent(p0.x(), p0.y(),p1.x(), p1.y(),p2.x(), p2.y(),p.x(), p.y(),u,v,w);
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

bool lineSegmentIntersection(
double Ax, double Ay,
double Bx, double By,
double Cx, double Cy,
double Dx, double Dy,
double *X, double *Y) {

  double  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line segment is zero-length.
  if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) return false;

  //  Fail if the segments share an end-point.
  if ((Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy)
  ||  (Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy)) {
    return false; }

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if segment C-D doesn't cross line A-B.
  if ((Cy<0. && Dy<0.) || (Cy>=0. && Dy>=0.)) return false;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos<0. || ABpos>distAB) return false;

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  *X=Ax+ABpos*theCos;
  *Y=Ay+ABpos*theSin;

  //  Success.
  return true; }


void nearest(QVector2D first, QVector2D last, QVector<QVector2D>& uv, int & nearestFirst, int& nearestLast) {

    //find the first
    float distFirst = FLT_MAX;
    float distLast = FLT_MAX;
    for (int i = 0; i< uv.size(); ++i) {
        float lf = (first - uv[i]).length();
        float ll = (last - uv[i]).length();
        if (lf < distFirst) {
            distFirst = lf;
            nearestFirst = i;
        }
        if (ll < distLast) {
            distLast = ll;
            nearestLast = i;
        }
    }
}

void insertInto(QVector<QVector2D>& vec,QVector<QVector2D>& in,int first, int last, bool replace) {

    QVector<QVector2D> tmp;

    if (first>last) {
        std::reverse(in.begin(), in.end());
        int tmpFirst = first;
        first = last;
        last = tmpFirst;
    }


    if(replace) {
        vec = in;
    } else {
        for (int i = 0; i<=first; ++i) {
            tmp.push_back(vec[i]);
        }


        for (int i = 0 ; i<in.size(); ++i) {
            tmp.push_back(in[i]);
        }

        for (int i = last;i <vec.size(); ++i) {
            tmp.push_back(vec[i]);
        }
        vec = tmp;
    }
}

float minimum_distance(QVector2D v, QVector2D w, QVector2D p) {
  // Return minimum distance between line segment vw and point p
  float l2 = (w-v).length();  // i.e. |w-v|^2 -  avoid a sqrt
  l2 = l2 * l2;
  if (l2 == 0.0) return (p - v).length();   // v == w case
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line.
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  const float t = QVector2D::dotProduct((p - v),( w - v)) / l2;
  if (t < 0.0) return (p- v).length();       // Beyond the 'v' end of the segment
  else if (t > 1.0) return (p- w).length();  // Beyond the 'w' end of the segment
  const QVector2D projection = v + t * (w - v);  // Projection falls on the segment
  return (p - projection).length();
}
