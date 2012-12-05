#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QComboBox>
#include "scene.h"
#include "sketchglwidget.h"
#include "sketchpad.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGLFormat * glFormat;
    QComboBox * layerChooser;
    MyGLWidget * gl;
private slots:
    void updateLayerChooser(Scene * s);
    void editLayer();
    void updateMenu(Scene *, BaseNode*);
    void pushScene(QAction * a) {
        if (a->text() != "Undo") {
            gl->pushScene();
        }
    }
};

#endif // MAINWINDOW_H
