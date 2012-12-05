#ifndef SKETCHGLWIDGET_H
#define SKETCHGLWIDGET_H

#include <QGLWidget>
#include "geometry/scene.h"
#include <QKeyEvent>
#include <QMap>
#include <QStack>
#include <iostream>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT

private:
    Scene *scene;
    QMap<int,bool> keys;
    QMap<int,bool> mouse;
    int previousMouseX;
    int previousMouseY;
    bool mouseMoved;
    float aspect;
    float move;
    QStack<Scene*> stack;


    QVector4D light_diffuse;
    QVector4D light_ambient;
    QVector4D light_specular;
    QVector4D light_position1;
    QVector4D light_position2;
    QVector4D light_position3;

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

    Vector3 findMouseDirection(QMouseEvent *e);

    void resizeGL(int w, int h);

    void checkInput();




    void setColor( QColor c);


    void addActions(QToolBar* menu);

signals:
    void itemSelected(Scene *, BaseNode *);

public slots:
    void pushScene();
    void setLayer(int);
    void animate();
    void makeLayer();
    void newLayer() {
        scene->activeNode = scene->boxNode;
    }
    void makeRiver();
    void makeRidge();
    void editLayer();
    void undo();
    void save();
    void open();
    void exportObj();
    void seaLevel();
    void createDeposits();

    void toggleVisibility();

    void setColor();
    void showSea(bool);

};

#endif // SKETCHGLWIDGET_H
