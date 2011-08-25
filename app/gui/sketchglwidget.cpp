#include <GL/glew.h>
#include "sketchglwidget.h"
#include "geometry/arrays.h"
#include <iostream>
#include <QTimer>
#include "geometry/node.h"

SketchGLWidget::SketchGLWidget(QGLFormat * glf, QWidget *parent) :
        QGLWidget(*glf,parent), move(0.01f)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor( QCursor( Qt::BlankCursor ) );
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(25);

}

void SketchGLWidget::checkInput() {
    static float multiplier = 1.0f;

    bool moving = false;
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
        scene.camera.goForward(move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_A)) {
        scene.camera.goRight(-move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_S)) {
        scene.camera.goForward(-move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_D)) {
        scene.camera.goRight(move*multiplier);
        moving = true;
    }

    if (moving) {
        multiplier += 0.5f;
    }else {
        multiplier = 1.0f;
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
        paintNode(node);
    }

//    glBegin(GL_TRIANGLES);
//    glVertex3f(-1.0,0.0,0.0);
//    glVertex3f(1.0,0.0,0.0);
//    glVertex3f(0.0,1.0,0.0);
//    glEnd();
}

void SketchGLWidget::paintNode(Node* node) {

    foreach(Node* node, node->children) {
        paintNode(node);
    }

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

    foreach (Spline* spline, node->splines) {
        for(int i = 0; i < spline->points.size(); ++i) {
            glPushMatrix();
            glTranslatef(spline->points[i].x(),spline->points[i].y(),spline->points[i].z());
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glCallList(scene.cursorSphere->displayList);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
        }
        if (spline->points.size() >= 4) {
            for (int i= 1; i<spline->points.size()-2; ++i) {

                glLineWidth(5.0f);
                glBegin(GL_LINES);
                glColor3f(0,0,0);
                float t = 0.0f;
                do  {
                    Vector3 a = spline->katmullRom(t,spline->points[i-1],spline->points[i],spline->points[i+1],spline->points[i+2]);
                    t+=0.1f;
                    Vector3 b = spline->katmullRom(t,spline->points[i-1],spline->points[i],spline->points[i+1],spline->points[i+2]);
                    glVertex3f(a.x(),a.y(),a.z());
                    glVertex3f(b.x(),b.y(),b.z());
                } while (t < 0.99f);

                glEnd();
            }
        }
    }
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
