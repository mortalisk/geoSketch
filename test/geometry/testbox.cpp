#include "testbox.h"
#include "geometry/box.h"

TestBox::TestBox(QObject *parent) :
    QObject(parent)
{
}

void TestBox::boxInit() {
    Box box(3.0f, 4.0f, 5.0f);
    QCOMPARE(box.getWidth(),3.0f);
    QCOMPARE(box.getHeight(),4.0f);
    QCOMPARE(box.getDepth(),5.0f);
}

