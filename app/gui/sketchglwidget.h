#ifndef SKETCHGLWIDGET_H
#define SKETCHGLWIDGET_H

#include <QGLWidget>
#include "geometry/box.h"
#include "geometry/scene.h"
#include <QKeyEvent>
#include <QMap>
#include <iostream>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT

private:
    Scene scene;
    QMap<int,bool> keys;
    QMap<int,bool> mouse;
    int previousMouseX;
    int previousMouseY;
    float aspect;
    float move;

public:
    explicit MyGLWidget(QGLFormat *glf,QWidget *parent = 0);
    void paintGL();
    void initializeGL();

    void enterEvent(QEvent *);

    void keyPressEvent(QKeyEvent * e);

    void keyReleaseEvent(QKeyEvent * e);

    bool isKeyPressed(int key);

    void mousePressEvent(QMouseEvent * e);

    void mouseReleaseEvent(QMouseEvent * e);

    bool isMousePressed(int button);

    void addPoint(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);

    void wheelEvent(QWheelEvent *e);


    void resizeGL(int w, int h);

    void checkInput();

signals:

public slots:
    void animate();
    void makeLayer();

};

#endif // SKETCHGLWIDGET_H
