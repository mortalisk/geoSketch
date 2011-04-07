#include <QtGui/QApplication>
#include <QtTest/QTest>
#include <geometry/testbox.h>
#include <geometry/testarrays.h>

bool runTests(int argc, char *argv[]) {
    TestBox testBox;
    QTest::qExec(&testBox, argc, argv);
    TestArrays testArrays;
    QTest::qExec(&testArrays, argc, argv);
    return true;
}

int main(int argc, char *argv[]) {
    //QApplication a(argc, argv);

    runTests(argc, argv);

    return 0;
    //return a.exec();
}
