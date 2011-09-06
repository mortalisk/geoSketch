#include "GL/glew.h"
#include <QtGui/QApplication>
#include <QtTest/QTest>
#include <geometry/testbox.h>
#include <geometry/testarrays.h>
#include <iostream>

bool runTests(int argc, char *argv[]) {
    TestBox testBox;
    QTest::qExec(&testBox, argc, argv);
    TestArrays testArrays;
    QTest::qExec(&testArrays, argc, argv);
    return true;
}

int main(int argc, char *argv[]) {
    //QApplication a(argc, argv);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      std::cerr <<  "Error: " << glewGetErrorString(err) << std::endl;
    }

    runTests(argc, argv);

    return 0;
    //return a.exec();
}
