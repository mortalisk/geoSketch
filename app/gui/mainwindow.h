#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QComboBox>
#include "scene.h"

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
private slots:
    void updateLayerChooser(Scene * s);
};

#endif // MAINWINDOW_H
