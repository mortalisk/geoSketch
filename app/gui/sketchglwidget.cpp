#include <GL/glew.h>
#include "sketchglwidget.h"
#include "geometry/arrays.h"
#include <iostream>
#include <QTimer>
#include "geometry/node.h"

MyGLWidget::MyGLWidget(QGLFormat * glf, QWidget *parent) :
        QGLWidget(*glf,parent), move(0.01f)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor( QCursor( Qt::BlankCursor ) );
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(25);

    scene = new Scene();

    pushScene();
}

void MyGLWidget::checkInput() {
    static float multiplier = 1.0f;

    bool moving = false;
    if (isKeyPressed(Qt::Key_Up)) {
        scene->camera.pitch(0.05);
    }
    if (isKeyPressed(Qt::Key_Left)) {
        scene->camera.yaw(0.05);
    }
    if (isKeyPressed(Qt::Key_Right)) {
        scene->camera.yaw(-0.05);
    }
    if (isKeyPressed(Qt::Key_Down)) {
        scene->camera.pitch(-0.05);
    }
    if (isKeyPressed(Qt::Key_W)) {
        scene->camera.goForward(move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_A)) {
        scene->camera.goRight(-move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_S)) {
        scene->camera.goForward(-move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_D)) {
        scene->camera.goRight(move*multiplier);
        moving = true;
    }

    if (moving) {
        multiplier += 0.5f;
    }else {
        multiplier = 1.0f;
    }
}

void MyGLWidget::paintGL() {
    checkInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(scene->camera.fov*180/M_PI, aspect, 0.1, 1000);
    gluLookAt(scene->camera.position.x(),
              scene->camera.position.y(),
              scene->camera.position.z(),
              scene->camera.looksAt().x(),
              scene->camera.looksAt().y(),
              scene->camera.looksAt().z(),
              scene->camera.up.x(),
              scene->camera.up.y(),
              scene->camera.up.z());

    scene->cursor->draw();
    scene->getRootNode()->draw();
}

void MyGLWidget::initializeGL() {
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      std::cerr <<  "Error: " << glewGetErrorString(err) << std::endl;
    }

    glClearColor(0.7,0.7,1.0,1.0);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;

    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* diffuse light. */
    GLfloat light_ambient[] = {0.9, 0.9, 0.9, 1.0};  /* ambient light. */
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */


    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void MyGLWidget::enterEvent(QEvent *) {
    setFocus();
}

void MyGLWidget::keyPressEvent(QKeyEvent * e) {
    if (!e->isAutoRepeat()) {
        keys[e->key()] = true;
    }

}

void MyGLWidget::keyReleaseEvent(QKeyEvent * e) {
    if (!e->isAutoRepeat()) {
        keys[e->key()] = false;
    }
}

bool MyGLWidget::isKeyPressed(int key) {
    return keys[key];
}

void MyGLWidget::mousePressEvent(QMouseEvent * e) {
    mouse[e->button()] = true;


    previousMouseX = e->x();
    previousMouseY = e->y();
    if (isMousePressed(Qt::LeftButton)) {
        std::cout << "pushing scene to stack" << std::endl;

        pushScene();
        addPoint(e);
    }
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent * e) {
    mouse[e->button()] = false;
    scene->activeNode->determineActionOnStoppedDrawing();

}

void MyGLWidget::pushScene() {
    stack.push(new Scene(*scene));
    emit sceneChanged(scene);
}

bool MyGLWidget::isMousePressed(int button) {
    return mouse[button];
}

void MyGLWidget::addPoint(QMouseEvent *e) {
    Vector3 dir = findMouseDirection(e);
    scene->addPoint(scene->camera.position, dir);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
    if (isMousePressed(Qt::RightButton)) {
        int movex = e->x() -previousMouseX;
        int movey = e->y() -previousMouseY;
        scene->camera.goUp(movey/100.0);
        scene->camera.goRight(-movex/100.0);
    }

    Vector3 dir = findMouseDirection(e);
    this->scene->showCursor(scene->camera.position,dir);


    if (isMousePressed(Qt::LeftButton)) {
       addPoint(e);
    }

    previousMouseX = e->x();
    previousMouseY = e->y();
}

Vector3 MyGLWidget::findMouseDirection(QMouseEvent *e) {
    // cursor move
    float a = scene->camera.fov/2.0f;
    float h = height()/2.0f;
    float w = width()/2.0f;
    float l = h/tan(a);
    Vector3 forw = scene->camera.forward.normalize()*l;
    float u = h - e->y();
    float r = e->x() - w;
    Vector3 up = scene->camera.up.normalize()*u;
    Vector3 right = forw.cross(scene->camera.up).normalize()*r;
    return scene->camera.position + forw + up + right;
}

void MyGLWidget::resizeGL(int w, int h) {
    aspect = w/(float)h;
    glViewport(0,0,w,h);
}

void MyGLWidget::wheelEvent(QWheelEvent *e) {
    scene->camera.goForward(-e->delta()/40.0f);
}

void MyGLWidget::makeLayer() {
    pushScene();
    scene->makeLayer();
    emit sceneChanged(scene);
}

void MyGLWidget::animate() {
        updateGL();
}

void MyGLWidget::setLayer(int i) {
    scene->activeNode = scene->boxNode->children[i];
}

void MyGLWidget::toggleVisibility(int i) {
    scene->boxNode->children[i]->visible = !scene->boxNode->children[i]->visible;
}

