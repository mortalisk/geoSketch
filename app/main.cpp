#include <QtGui/QApplication>
#include "mainwindow.h"
#include <string>

bool runTests(int argc, char *argv[]) {
   // TestBox testBox;
   // QTest::qExec(&testBox, argc, argv);
    return true;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow * w;
    if(false) {
        runTests(argc, argv);
    }else {
        w = new MainWindow;
        w->show();
    }

    return a.exec();
}


