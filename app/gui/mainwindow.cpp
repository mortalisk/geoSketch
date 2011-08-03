#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QGLWidget>
#include "sketchglwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout * mainLayout = new QHBoxLayout();
    centralWidget()->setLayout(mainLayout);

    glFormat = new QGLFormat;
    glFormat->setProfile(QGLFormat::CompatibilityProfile);//CoreProfile);
    glFormat->setVersion(3,3);
    SketchGLWidget * gl = new SketchGLWidget(glFormat);
    mainLayout->addWidget(gl);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glFormat;
}
