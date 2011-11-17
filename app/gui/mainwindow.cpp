#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGLWidget>
#include <QPushButton>
#include "sketchglwidget.h"
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout * mainLayout = new QVBoxLayout();
    QToolBar * toolBar = new QToolBar();
    centralWidget()->setLayout(mainLayout);


    QPushButton * undoButton = new QPushButton("UNDO");
    toolBar->addWidget(undoButton);
    QPushButton * layerButton = new QPushButton("Make Layer");
    toolBar->addWidget(layerButton);
    QPushButton * newLayerButton = new QPushButton("New Layer");
    toolBar->addWidget(newLayerButton);

    mainLayout->addWidget(toolBar);

    glFormat = new QGLFormat;
    glFormat->setProfile(QGLFormat::CompatibilityProfile);//CoreProfile);
    glFormat->setVersion(3,3);
    MyGLWidget * gl = new MyGLWidget(glFormat);

    QObject::connect(undoButton,SIGNAL(clicked(bool)), gl, SLOT(undo()));
    QObject::connect(layerButton,SIGNAL(clicked(bool)), gl, SLOT(makeLayer()));
    QObject::connect(newLayerButton,SIGNAL(clicked(bool)), gl, SLOT(newLayer()));

    mainLayout->addWidget(gl);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glFormat;
}
