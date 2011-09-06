#include "testbox.h"
#include "geometry/box.h"
#include <QVector>
#include <QVector3D>

TestBox::TestBox(QObject *parent) :
    QObject(parent)
{
}

void TestBox::boxConstructor() {
    Box box(3.0f, 4.0f, 5.0f);
    QCOMPARE(box.getWidth(),3.0f);
    QCOMPARE(box.getHeight(),4.0f);
    QCOMPARE(box.getDepth(),5.0f);
}

void TestBox::boxMesh() {
//    Box box(3.0f, 6.0f, 2.0f);
//    Shape * shape = &box;
//    QVector<QVector3D> expected(36);
//    QCOMPARE(shape->getVertices().size(), expected.size());
}


