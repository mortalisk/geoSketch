#ifndef SKETCHGLWIDGET_H
#define SKETCHGLWIDGET_H

#include <QGLWidget>

class SketchGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit SketchGLWidget(QGLFormat *glf,QWidget *parent = 0);
    void paintGL();
    void initializeGL();

signals:

public slots:

};

#endif // SKETCHGLWIDGET_H
