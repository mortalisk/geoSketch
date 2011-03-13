#include "sketchglwidget.h"

SketchGLWidget::SketchGLWidget(QGLFormat * glf, QWidget *parent) :
        QGLWidget(*glf,parent)
{
}

void SketchGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SketchGLWidget::initializeGL() {
    glClearColor(0.7,0.7,1.0,1.0);
}
