#include <GL/glew.h>
#include "sketchglwidget.h"
#include "geometry/arrays.h"
#include <iostream>
#include <QTimer>
#include "geometry/node.h"

SketchGLWidget::SketchGLWidget(QGLFormat * glf, QWidget *parent) :
        QGLWidget(*glf,parent)
{
    setFocusPolicy(Qt::StrongFocus);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(25);

}

void SketchGLWidget::checkInput() {

    if (isKeyPressed(Qt::Key_Up)) {
        scene.camera.pitch(0.05);
    }
    if (isKeyPressed(Qt::Key_Left)) {
        scene.camera.yaw(0.05);
    }
    if (isKeyPressed(Qt::Key_Right)) {
        scene.camera.yaw(-0.05);
    }
    if (isKeyPressed(Qt::Key_Down)) {
        scene.camera.pitch(-0.05);
    }
    if (isKeyPressed(Qt::Key_W)) {
        scene.camera.goForward(0.5);
    }
    if (isKeyPressed(Qt::Key_A)) {
        scene.camera.goRight(-0.5);
    }
    if (isKeyPressed(Qt::Key_S)) {
        scene.camera.goForward(-0.5);
    }
    if (isKeyPressed(Qt::Key_D)) {
        scene.camera.goRight(0.5);
    }
}

void SketchGLWidget::paintGL() {
    checkInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(scene.camera.fov*180/M_PI, aspect, 0.1, 1000);
    gluLookAt(scene.camera.position.x(),
              scene.camera.position.y(),
              scene.camera.position.z(),
              scene.camera.looksAt().x(),
              scene.camera.looksAt().y(),
              scene.camera.looksAt().z(),
              scene.camera.up.x(),
              scene.camera.up.y(),
              scene.camera.up.z());

    foreach(Node* node, scene.getNodes()) {
        Shape * shape = node->shape;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glTranslatef(node->position.x(),node->position.y(),node->position.z());
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        if (shape->displayList == -1 ) {
            // create one display list
            shape->displayList = glGenLists(1);

            // compile the display list
            glNewList(shape->displayList, GL_COMPILE);

            glVertexPointer(3,GL_FLOAT,sizeof(vertex),&shape->getVertices()[0]);
            glColorPointer(4,GL_FLOAT,sizeof(vertex),&shape->getVertices()[0].r);
            glDrawArrays(GL_TRIANGLES,0,shape->getVertices().size());

            glEndList();
            // delete it if it is not used any more
            //
        }
        glCallList(shape->displayList);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }

//    glBegin(GL_TRIANGLES);
//    glVertex3f(-1.0,0.0,0.0);
//    glVertex3f(1.0,0.0,0.0);
//    glVertex3f(0.0,1.0,0.0);
//    glEnd();
}

void SketchGLWidget::initializeGL() {
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      std::cerr <<  "Error: " << glewGetErrorString(err) << std::endl;
    }

    glClearColor(0.7,0.7,1.0,1.0);
    glEnable(GL_DEPTH_TEST);
}
