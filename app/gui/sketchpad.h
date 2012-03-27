#ifndef SKETCHPAD_H
#define SKETCHPAD_H
#include <QPainter>
#include <QWidget>
#include "geometry/spline.h"
#include <QVector>
#include <QMouseEvent>

class SketchPad : public QWidget
{
private:
    QVector<QPointF> line;
public:
    SketchPad();

    void mousePressEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent *);

    void paintEvent(QPaintEvent * e);
};

#endif // SKETCHPAD_H
