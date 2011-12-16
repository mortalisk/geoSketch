#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGLWidget>
#include <QPushButton>
#include "sketchglwidget.h"
#include <QToolBar>
#include <QComboBox>

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
    layerChooser = new QComboBox();
    toolBar->addWidget(layerChooser);

    mainLayout->addWidget(toolBar);

    glFormat = new QGLFormat;
    glFormat->setProfile(QGLFormat::CompatibilityProfile);//CoreProfile);
    glFormat->setVersion(3,3);
    MyGLWidget * gl = new MyGLWidget(glFormat);

    QObject::connect(undoButton,SIGNAL(clicked(bool)), gl, SLOT(undo()));
    QObject::connect(layerButton,SIGNAL(clicked(bool)), gl, SLOT(makeLayer()));
    QObject::connect(newLayerButton,SIGNAL(clicked(bool)), gl, SLOT(newLayer()));
    QObject::connect(gl,SIGNAL(sceneChanged(Scene *)), this, SLOT(updateLayerChooser(Scene *)));
    QObject::connect(layerChooser,SIGNAL(activated(int)), gl, SLOT(setLayer(int)));

    mainLayout->addWidget(gl);
}

void MainWindow::updateLayerChooser(Scene * s) {
    int i = 1;
    std::cout << "update layers: " << s->boxNode->children.size() << std::endl;
    layerChooser->clear();
    QString index;
    int selected = 0;
    foreach(Node *surf, s->boxNode->children) {
        if (surf == s->activeNode) {
            selected = i;
        }
        index.setNum(i);
        layerChooser->insertItem(i, surf->name + index);
        i++;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glFormat;
}
