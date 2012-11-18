#ifndef CURVE_H
#define CURVE_H
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class Curve
{
public:
    QVector<QVector2D> original2dPoints;
    QVector<QVector2D> bezier2dPoints;

    void addPoint(QVector2D point) {
        original2dPoints.push_back(point);
    }
    void oversketch(Curve curve);
    void extend();

    Curve();
};

#endif // CURVE_H
