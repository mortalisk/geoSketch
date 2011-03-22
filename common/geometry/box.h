#ifndef BOX_H
#define BOX_H

#include <QObject>
#include <QVector>
#include <QVector3D>

class Box : public QObject
{
    Q_OBJECT
private:
    float width, heigth, depth;
    QVector<QVector3D> vertices;
public:
    explicit Box(float width, float heigth, float depth, QObject *parent = 0);
    float getWidth();
    float getHeight();
    float getDepth();

    QVector<QVector3D> & getVertices();

signals:

public slots:

};

#endif // BOX_H
