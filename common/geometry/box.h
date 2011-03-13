#ifndef BOX_H
#define BOX_H

#include <QObject>

class Box : public QObject
{
    Q_OBJECT
private:
    float width, heigth, depth;
public:
    explicit Box(float width, float heigth, float depth, QObject *parent = 0);
    float getWidth();
    float getHeight();
    float getDepth();

signals:

public slots:

};

#endif // BOX_H
