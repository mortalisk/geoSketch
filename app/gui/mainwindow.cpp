#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGLWidget>
#include <QPushButton>
#include "sketchglwidget.h"
#include <QToolBar>
#include <QComboBox>
#include <QCheckBox>
#include <QColorDialog>

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

    QPushButton * toggleVisibility = new QPushButton("Visibility");
    toolBar->addWidget(toggleVisibility);
    QPushButton * colorButton = new QPushButton("Color");
    toolBar->addWidget(colorButton);

    mainLayout->addWidget(toolBar);

    glFormat = new QGLFormat;
    glFormat->setProfile(QGLFormat::CompatibilityProfile);//CoreProfile);
    glFormat->setVersion(3,3);
    gl = new MyGLWidget(glFormat);

    QObject::connect(undoButton,SIGNAL(clicked(bool)), gl, SLOT(undo()));
    QObject::connect(layerButton,SIGNAL(clicked(bool)), gl, SLOT(makeLayer()));
    QObject::connect(newLayerButton,SIGNAL(clicked(bool)), gl, SLOT(newLayer()));
    QObject::connect(gl,SIGNAL(sceneChanged(Scene *)), this, SLOT(updateLayerChooser(Scene *)));
    QObject::connect(layerChooser,SIGNAL(activated(int)), gl, SLOT(setLayer(int)));
    QObject::connect(toggleVisibility,SIGNAL(clicked(bool)), this, SLOT(toggleVisibility()));
    QObject::connect(colorButton,SIGNAL(clicked(bool)), this, SLOT(setColor()));

    mainLayout->addWidget(gl);
}

void MainWindow::updateLayerChooser(Scene * s) {
    int i = 0;
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
    layerChooser->setCurrentIndex(selected);
}

void MainWindow::toggleVisibility() {
    gl->toggleVisibility(layerChooser->currentIndex());
}

void MainWindow::setColor() {
    QColor c = QColorDialog::getColor();
    gl->setColor(layerChooser->currentIndex(),c);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete glFormat;
}
