#include "sketchpad.h"

SketchPad::SketchPad()
{
}

void SketchPad::mousePressEvent(QMouseEvent * e) {

    if (e->button() == Qt::LeftButton) {
        float x = e->x()/(float)width() -0.5;
        float y = e->y()/(float)height() -0.5;
        line.push_back(QPointF(x, y));
        repaint();
    }
}

void SketchPad::mouseMoveEvent(QMouseEvent * e) {

    if (e->buttons()&&Qt::LeftButton) {
        float x = e->x()/(float)width() -0.5;
        float y = e->y()/(float)height() -0.5;
        line.push_back(QPointF(x, y));
        repaint();
    }
}

void SketchPad::paintEvent(QPaintEvent *) {
    QPen pen(Qt::black, 2, Qt::SolidLine);

    QPainter painter(this);
    painter.setPen(pen);
    painter.fillRect(0, 0, width(), height(), Qt::white);

    foreach (const QPointF& point, line) {
        painter.drawEllipse((point.x()+0.5)*width(),(point.y()+0.5)*height(),1,1);
    }
}
