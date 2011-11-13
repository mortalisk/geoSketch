#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGLWidget>
#include <QPushButton>
#include "sketchglwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout * mainLayout = new QVBoxLayout();
    QHBoxLayout * menuLayout = new QHBoxLayout();
    centralWidget()->setLayout(mainLayout);

    QPushButton * undoButton = new QPushButton("UNDO");
    menuLayout->addWidget(undoButton);
    mainLayout->addLayout(menuLayout);


    glFormat = new QGLFormat;
    glFormat->setProfile(QGLFormat::CompatibilityProfile);//CoreProfile);
    glFormat->setVersion(3,3);
    MyGLWidget * gl = new MyGLWidget(glFormat);

    QObject::connect(undoButton,SIGNAL(clicked(bool)), gl, SLOT(undo()));

    mainLayout->addWidget(gl);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glFormat;
}
