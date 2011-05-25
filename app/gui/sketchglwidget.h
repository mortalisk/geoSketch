#ifndef SKETCHGLWIDGET_H
#define SKETCHGLWIDGET_H

#include <QGLWidget>
#include "geometry/box.h"
#include "geometry/scene.h"
#include <QKeyEvent>
#include <QMap>
#include <iostream>

class SketchGLWidget : public QGLWidget
{
    Q_OBJECT

private:
    Scene scene;
    QMap<int,bool> keys;
    QMap<int,bool> mouse;
    int previousMouseX;
    int previousMouseY;
    float aspect;

public:
    explicit SketchGLWidget(QGLFormat *glf,QWidget *parent = 0);
    void paintGL();
    void initializeGL();

    void keyPressEvent(QKeyEvent * e) {
        if (!e->isAutoRepeat()) {
            keys[e->key()] = true;
        }

    }

    void keyReleaseEvent(QKeyEvent * e) {
        if (!e->isAutoRepeat()) {
            keys[e->key()] = false;
        }

    }

    bool isKeyPressed(int key) {
        return keys[key];
    }

    void mousePressEvent(QMouseEvent * e) {
        mouse[e->button()] = true;


        previousMouseX = e->x();
        previousMouseY = e->y();
    }

    void mouseReleaseEvent(QMouseEvent * e) {
        mouse[e->button()] = false;
    }

    bool isMousePressed(int button) {
        return mouse[button];
    }

    void mouseMoveEvent(QMouseEvent *e) {
        if (isMousePressed(Qt::RightButton)) {
            int movex = e->x() -previousMouseX;
            int movey = e->y() -previousMouseY;
            scene.camera.pitch(-movey/100.0);
            scene.camera.turn(-movex/100.0);
        }

        if (isMousePressed(Qt::LeftButton)) {
            float a = scene.camera.fov/2.0f;
            float h = height()/2.0f;
            float w = width()/2.0f;
            float l = h/tan(a);
            Vector3 forw = scene.camera.forward.normalize()*l;
            float u = h - e->y();
            float r = e->x() - w;
            Vector3 up = scene.camera.up.normalize()*u;
            Vector3 right = forw.cross(scene.camera.up).normalize()*r;
            Vector3 dir = scene.camera.position + forw + up + right;
            this->scene.addPoint(scene.camera.position,dir);
        }

        previousMouseX = e->x();
        previousMouseY = e->y();
    }

    void resizeGL(int w, int h) {
        aspect = w/(float)h;
        glViewport(0,0,w,h);
    }

    void checkInput();

signals:

public slots:
    void animate() {
            updateGL();
    }

};

#endif // SKETCHGLWIDGET_H
