#include "ridgenode.h"
#include "surfacenode.h"
#include "surface.h"

#include <GL/glew.h>

RidgeNode::RidgeNode(Spline& spline) : BaseNode("rigde")
{
    this->spline.points += spline.points;
    this->baseSpline.points += spline.points;
}

void RidgeNode::makeWall() {
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glBegin(GL_TRIANGLE_STRIP);
//    float color[4] = {0.7,0.4,0.4,1.0};
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);

    QVector<Vector3> triangles;
    QVector<Vector3> normals;

//    float spec[4] = {1.0,1.0,1.0,1.0};
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
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
//        glNormal3f(normal.x(),normal.y(),normal.z());
//        glVertex3d(current.x(),current.y(),current.z());
//        glVertex3d(current.x(),current.y()+5,current.z());


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

//        glNormal3f(normal.x(),normal.y(),normal.z());
//        glVertex3d(current.x(),current.y(),current.z());
//        glVertex3d(current.x(),current.y()+5,current.z());


    }*/
//    glEnd();
//    glDisable(GL_CULL_FACE);
    QVector<Vector3> noOutline;
    Surface* s = new Surface(triangles,normals, noOutline,true);
    shape = s;
}


void RidgeNode::doTransformSurface(SurfaceNode &surfacenode) {
    QVector<triangle> & triangles = surfacenode.shape->getTriangles();
    for(int i = 0 ;i <triangles.size();++i) {

    }
}

void RidgeNode::repositionOnSurface(SurfaceNode &surfacenode) {

}
