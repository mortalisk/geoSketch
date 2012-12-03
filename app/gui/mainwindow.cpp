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
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout * mainLayout = new QVBoxLayout();
    centralWidget()->setLayout(mainLayout);

    QToolBar * toolBar = findChild<QToolBar*>("toolBar");
    QAction * actionSave = findChild<QAction*>("actionSave");
    QAction * actionLoad = findChild<QAction*>("actionLoad");
    QAction * actionExport = findChild<QAction*>("actionExport");

    toolBar->clear();

    glFormat = new QGLFormat;
    glFormat->setProfile(QGLFormat::CompatibilityProfile);//CoreProfile);
    glFormat->setVersion(3,3);
    gl = new MyGLWidget(glFormat);

    gl->addActions(toolBar);

    connect(gl,SIGNAL(itemSelected(Scene *, BaseNode *)), this, SLOT(updateMenu(Scene*,BaseNode*)));
    connect(actionSave,SIGNAL(triggered()),gl,SLOT(save()));
    connect(actionLoad,SIGNAL(triggered()),gl,SLOT(open()));

    connect(actionExport,SIGNAL(triggered()),gl,SLOT(exportObj()));

    mainLayout->addWidget(gl);
}

void MainWindow::updateLayerChooser(Scene * s) {
    int i = 0;
    std::cout << "update layers: " << s->boxNode->children.size() << std::endl;
    layerChooser->clear();
    QString index;
    int selected = 0;
    foreach(BaseNode *surf, s->boxNode->children) {
        if (surf == s->activeNode) {
            selected = i;
        }
        index.setNum(i);
        layerChooser->insertItem(i, surf->name + index);
        i++;
    }
    layerChooser->setCurrentIndex(selected);
}



void MainWindow::updateMenu(Scene * scene, BaseNode* node) {

    QToolBar * toolBar = findChild<QToolBar*>("toolBar");

    toolBar->clear();


    gl->addActions(toolBar);
    node->addActions(toolBar);
}

void MainWindow::editLayer() {
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glFormat;
}
