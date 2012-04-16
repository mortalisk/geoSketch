#include <QVector>
#include "Vector3.h"
#include <QVector2D>

static const float EPSILON=0.0000000001f;

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
bool InsideTriangle(float Ax, float Ay,
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




enum Axis { X, Y, Z};

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
