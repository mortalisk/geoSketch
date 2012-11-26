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
    QToolBar * toolBar = findChild<QToolBar*>("toolBar");
    centralWidget()->setLayout(mainLayout);

    QAction * undoButton = findChild<QAction*>("actionUndo");
    QAction * layerButton = findChild<QAction*>("actionMakeLayer");
    QAction * newLayerButton = findChild<QAction*>("actionNewLayer");
    QAction * toggleVisibility = findChild<QAction*>("actionVisibility");
    QAction * colorButton = findChild<QAction*>("actionColor");
    QAction * editLayer = findChild<QAction*>("actionEditLayer");
    QAction * makeRidge = findChild<QAction*>("actionMakeRidge");
    QAction * makeRiver = findChild<QAction*>("actionMakeRiver");
    QAction * saveButton = findChild<QAction*>("actionSave");
    QAction * openButton = findChild<QAction*>("actionOpen");
    QAction * seaButton = findChild<QAction*>("actionSea");
    QAction * depositButton = findChild<QAction*>("actionDeposit");

//    layerChooser = new QComboBox();
//    toolBar->addWidget(layerChooser);

    glFormat = new QGLFormat;
    glFormat->setProfile(QGLFormat::CompatibilityProfile);//CoreProfile);
    glFormat->setVersion(3,3);
    gl = new MyGLWidget(glFormat);

    QObject::connect(undoButton,SIGNAL(activated()), gl, SLOT(undo()));
    QObject::connect(layerButton,SIGNAL(activated()), gl, SLOT(makeLayer()));
    QObject::connect(newLayerButton,SIGNAL(activated()), gl, SLOT(newLayer()));
    //QObject::connect(gl,SIGNAL(sceneChanged(Scene *)), this, SLOT(updateLayerChooser(Scene *)));

    QObject::connect(gl,SIGNAL(itemSelected(Scene *, BaseNode*)), this, SLOT(updateMenu(Scene *, BaseNode*)));
    //QObject::connect(layerChooser,SIGNAL(activated(int)), gl, SLOT(setLayer(int)));
    QObject::connect(toggleVisibility,SIGNAL(activated()), this, SLOT(toggleVisibility()));
    QObject::connect(colorButton,SIGNAL(activated()), this, SLOT(setColor()));
    QObject::connect(editLayer,SIGNAL(activated()), gl, SLOT(editLayer()));
    QObject::connect(makeRidge,SIGNAL(activated()), gl, SLOT(makeRidge()));
    QObject::connect(makeRiver,SIGNAL(activated()), gl, SLOT(makeRiver()));
    QObject::connect(saveButton,SIGNAL(activated()), gl, SLOT(save()));
    QObject::connect(openButton,SIGNAL(activated()), gl, SLOT(open()));
    QObject::connect(seaButton,SIGNAL(activated()), gl, SLOT(seaLevel()));
    QObject::connect(depositButton,SIGNAL(activated()), gl, SLOT(createDeposits()));

    QDockWidget * sketchDock = new QDockWidget();
    SketchPad * sketchPadSlice = new SketchPad();
    SketchPad * sketchPadProfile = new SketchPad();
    QLayout * sketchLayout = new QVBoxLayout();
    QWidget * sketchWidget = new QWidget();
    sketchWidget->setMinimumWidth(200);
    sketchWidget->setLayout(sketchLayout);
    sketchDock->setWidget(sketchWidget);

    sketchLayout->addWidget(sketchPadSlice);
    sketchLayout->addWidget(sketchPadProfile);
    sketchLayout->addWidget(new QPushButton("hei"));

    //this->addDockWidget(Qt::RightDockWidgetArea, sketchDock);

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

void MainWindow::toggleVisibility() {
    gl->toggleVisibility(layerChooser->currentIndex());
}

void MainWindow::setColor() {
    QColor c = QColorDialog::getColor();
    gl->setColor(layerChooser->currentIndex(),c);

}

void MainWindow::updateMenu(Scene *, BaseNode*) {

}

void MainWindow::editLayer() {
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glFormat;
}
