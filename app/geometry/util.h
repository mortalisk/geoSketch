#ifndef UTIL_H
#define UTIL_H

#include <QVector>
#include <QVector2D>
#include "Vector3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>



float Area(const QVector<Vector3> &contour);

   /*
     InsideTriangle decides if a point P is Inside of the triangle
     defined by A, B, C.
   */
bool InsideTriangle(float Ax, float Ay,
                      float Bx, float By,
                      float Cx, float Cy,
                      float Px, float Py);

bool Snip(const QVector<Vector3> &contour,int u,int v,int w,int n,int *V);

bool Process(const QVector<Vector3> &contour, const QVector<Vector3> &contour3d,QVector<Vector3> &result);

enum Axis { X, Y, Z};

bool triangulate(const QVector<Vector3> &contour,QVector<Vector3> &result, Axis axis );

bool similar(float a, float b);

bool similar(float a, float b, float c);

// compute the area of a triangle using Heron's formula
float triarea(float a, float b, float c);

// compute the distance between two 2d points
float dist(float x0, float y0, float x1, float y1);

// compute the barycentric coordinates of a 2d point inside a 2d triangle
// (x0, y0) (x1, y1) (x2, y2) are the vertices of a 2d triangle
// (vx, vy) is a point inside the 2d triangle
// u, v, w are the barycentric coordinates of (vx, vy) in the triangle
void barycent(float x0, float y0, float x1, float y1, float x2, float y2,
    float vx, float vy,
    float *u, float *v, float *w);

void  barycent(QVector2D p0, QVector2D p1, QVector2D p2, QVector2D p, float *u, float *v, float *w );


float sideOfLine(QVector2D A, QVector2D B, QVector2D P);

bool inside(QVector2D A, QVector2D B, QVector2D C, QVector2D P, float* u, float*v);

bool inside(QVector2D A,QVector2D B,QVector2D C,QVector2D D, QVector2D point);

float lowest(float a, float b, float c, float d);

float highest(float a, float b, float c, float d);

QVector2D linesIntersection(QVector2D& p1, QVector2D& p2, QVector2D& p3, QVector2D& p4);

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

inline double round( double d )
{
    return floor( d + 0.5 );
}

#endif // UTIL_H
