#include <QtGui/QApplication>
#include "mainwindow.h"
#include <string>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow * w;

        w = new MainWindow;
        w->show();


    return a.exec();
}


