#include <GL/glew.h>
#include "surfacenode.h"
#include "sketchglwidget.h"
#include "geometry/arrays.h"
#include <iostream>
#include <QTimer>
#include "geometry/basenode.h"
#include <QVector>
#include "json.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include "rivernode.h"
#include <QMenu>
#include <QColor>
#include <QColorDialog>
#include "deposit.h"

MyGLWidget::MyGLWidget(QGLFormat * glf, QWidget *parent) :
        QGLWidget(*glf,parent), move(0.01f)
{


    light_diffuse = QVector4D(0.33, 0.33, 0.33, 1.0);
    light_ambient = QVector4D(0.2, 0.2, 0.2, 1.0);  /* light. */
    light_specular = QVector4D(0.0,0.0,0.0, 1.0);
    light_position1 = QVector4D(0.0, 1.0, 1.0, 0.0);  /* Infinite light location. */
    light_position2 = QVector4D(1.0, 1.0, -1.0, 0.0);
    light_position3 = QVector4D(-1.0, 1.0, -1.0, 0.0);

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor( QCursor( Qt::CrossCursor ) );
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(25);

    scene = new Scene();

    pushScene();
}

void MyGLWidget::checkInput() {
    static float multiplier = 1.0f;

    bool moving = false;
    if (isKeyPressed(Qt::Key_Up)) {
        scene->camera.trackCenter += Vector3(0,0.1,0);
        scene->camera.fix();
    }
    if (isKeyPressed(Qt::Key_Left)) {
        scene->camera.trackCenter = scene->camera.trackCenter - scene->camera.forward.cross(scene->camera.up).normalize()*0.1;
        scene->camera.fix();
    }
    if (isKeyPressed(Qt::Key_Right)) {
        scene->camera.trackCenter = scene->camera.trackCenter + scene->camera.forward.cross(scene->camera.up).normalize()*0.1;
        scene->camera.fix();
    }
    if (isKeyPressed(Qt::Key_Down)) {
        scene->camera.trackCenter += Vector3(0,-0.1,0);
        scene->camera.fix();
    }
    if (isKeyPressed(Qt::Key_W)) {
        scene->camera.goForward(move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_A)) {
        scene->camera.goRight(-move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_S)) {
        scene->camera.goForward(-move*multiplier);
        moving = true;
    }
    if (isKeyPressed(Qt::Key_D)) {
        scene->camera.goRight(move*multiplier);
        moving = true;
    }

    if (moving) {
        multiplier += 0.5f;
    }else {
        multiplier = 1.0f;
    }
}

void MyGLWidget::paintGL() {
    checkInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(scene->camera.fov*180/M_PI, aspect, 0.1, 1000);

    if(isKeyPressed(Qt::Key_T)) {
         glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(scene->camera.position.x(),
              scene->camera.position.y(),
              scene->camera.position.z(),
              scene->camera.looksAt().x(),
              scene->camera.looksAt().y(),
              scene->camera.looksAt().z(),
              scene->camera.up.x(),
              scene->camera.up.y(),
              scene->camera.up.z());

    float lightDiffuse[4] = {light_diffuse.x(), light_diffuse.y(), light_diffuse.z(), light_diffuse.w()};  /* diffuse light. */
    float lightAmbient[4] = {light_ambient.x(), light_ambient.y(), light_ambient.z(), light_ambient.w()};  /* ambient light. */
    float lightSpecular[4] = {light_specular.x(), light_specular.y(), light_specular.z(), light_specular.w()};  /* ambient light. */
    float lightPosition1[4] = {light_position1.x(), light_position1.y(), light_position1.z(), light_position1.w()}; /* Infinite light location. */
    float lightPosition2[4] = {light_position2.x(), light_position2.y(), light_position2.z(), light_position2.w()}; /* Infinite light location. */
    float lightPosition3[4] = {light_position3.x(), light_position3.y(), light_position3.z(), light_position3.w()}; /* Infinite light location. */


    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);

    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

    scene->getRootNode()->draw();
    scene->cursor->draw();
}

void MyGLWidget::initializeGL() {
//    GLenum err = glewInit();
//    if (GLEW_OK != err)
//    {
//      /* Problem: glewInit failed, something is seriously wrong. */
//      std::cerr <<  "Error: " << glewGetErrorString(err) << std::endl;
//    }

    glClearColor(0.7,0.7,1.0,1.0);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    //glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;




    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_NORMALIZE);

}

void MyGLWidget::enterEvent(QEvent *) {
    setFocus();
}

void MyGLWidget::keyPressEvent(QKeyEvent * e) {
    if (!e->isAutoRepeat()) {
        keys[e->key()] = true;
    }

}

void MyGLWidget::keyReleaseEvent(QKeyEvent * e) {
    if (!e->isAutoRepeat()) {
        keys[e->key()] = false;
    }
}

bool MyGLWidget::isKeyPressed(int key) {
    return keys[key];
}

void MyGLWidget::mousePressEvent(QMouseEvent * e) {
    mouse[e->button()] = true;

    mouseMoved = false;
    previousMouseX = e->x();
    previousMouseY = e->y();
    if (isMousePressed(Qt::LeftButton)) {
        addPoint(e);
    }
}



void MyGLWidget::addActions(QToolBar* menu) {
    QAction * actionUndo = new QAction("Undo", menu);
    menu->addAction(actionUndo);
    connect(actionUndo,SIGNAL(triggered()), this, SLOT(undo()));

    if (scene->activeNode) {
        QAction * actionVisible = new QAction("Visible", menu);
        actionVisible->setCheckable(true);
        actionVisible->setChecked(scene->activeNode->visible);
        menu->addAction(actionVisible);
        connect(actionVisible,SIGNAL(triggered()), this, SLOT(toggleVisibility()));

        QAction * actionColor = new QAction("Color", menu);
        menu->addAction(actionColor);
        connect(actionColor,SIGNAL(triggered()), this, SLOT(setColor()));

    }

    menu->addSeparator();

    SurfaceNode * sn = qobject_cast<SurfaceNode*>(scene->activeNode);

    if (sn||scene->editLayerNo != -1) {
        QAction * editLayer = new QAction(QString("Editing"), menu);
        editLayer->setCheckable(true);
        editLayer->setChecked(scene->editLayerNo != -1);
        connect(editLayer, SIGNAL(triggered()), this, SLOT(editLayer()));
        menu->addAction(editLayer);
    } else {
        BoxNode * bn = qobject_cast<BoxNode*>(scene->activeNode);
        if (bn) {
            QAction * showSea = new QAction(QString("Change Sea"), menu);
            showSea->setCheckable(true);
            showSea->setChecked(scene->boxNode->waterNode->visible);
            connect(showSea, SIGNAL(toggled(bool)), this, SLOT(seaLevel()));
            menu->addAction(showSea);
        } else {
            RiverNode * rn =  qobject_cast<RiverNode*>(scene->activeNode);

            if (rn) {
                QAction * deposit = new QAction(QString("New Deposit"), menu);
                connect(deposit, SIGNAL(triggered()), this, SLOT(createDeposits()));
                menu->addAction(deposit);

                if (deposit) {
                    QAction * stop = new QAction(QString("Stop/start Deposit"), menu);
                    connect(stop, SIGNAL(triggered()), rn, SLOT(stopDeposit()));
                    menu->addAction(stop);
                }

            }

        }
    }

}

void MyGLWidget::showSea(bool show) {
    scene->boxNode->waterNode->visible = show;
}

void MyGLWidget::setColor() {
    QColor c = QColorDialog::getColor();
    setColor(c);

}

void MyGLWidget::mouseReleaseEvent(QMouseEvent * e) {
    mouse[e->button()] = false;



    if (!mouseMoved && e->button() == Qt::RightButton) {
        Vector3 dir = findMouseDirection(e);
        BaseNode * previous = scene->activeNode;
        scene->selectActiveNode(scene->camera.position, dir);
        if (scene->activeNode)
            emit(itemSelected(scene,scene->activeNode));
//        if (scene->activeNode && scene->activeNode == previous) {
//            QMenu context(this);
//            scene->activeNode->addActions(&context);
//            context.exec(e->globalPos());
//        }
    }

    if (mouseMoved && e->button() == Qt::LeftButton) {
        scene->activeNode->determineActionOnStoppedDrawing();

        pushScene();
    }
}

void MyGLWidget::pushScene() {
    std::cout << "PUSHING!!" << std::endl;
    stack.push(new Scene(*scene));
    if (scene->activeNode)
        emit itemSelected(scene, scene->activeNode);
}

void MyGLWidget::undo(){
    Camera c = scene->camera;
    if (stack.size() > 1) {
        delete scene;
        scene = stack.pop();
        delete scene;
        scene = stack.pop();
    }
    pushScene();

    scene->camera = c;

    if (scene->activeNode)
        emit itemSelected(scene, scene->activeNode);
}

bool MyGLWidget::isMousePressed(int button) {
    return mouse[button];
}

void MyGLWidget::addPoint(QMouseEvent *e) {
    Vector3 dir = findMouseDirection(e);
    scene->addPoint(scene->camera.position, dir);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
    if (isMousePressed(Qt::RightButton)) {
        int movex = e->x() -previousMouseX;
        int movey = e->y() -previousMouseY;
        scene->camera.goUp(movey/100.0);
        scene->camera.goRight(-movex/100.0);
    }

    Vector3 dir = findMouseDirection(e);
    this->scene->showCursor(scene->camera.position,dir);
    mouseMoved = true;

    if (isMousePressed(Qt::LeftButton)) {
       addPoint(e);
    }

    previousMouseX = e->x();
    previousMouseY = e->y();
}

Vector3 MyGLWidget::findMouseDirection(QMouseEvent *e) {
    // cursor move
    float a = scene->camera.fov/2.0f;
    float h = height()/2.0f;
    float w = width()/2.0f;
    float l = h/tan(a);
    Vector3 forw = scene->camera.forward.normalize()*l;
    float u = h - e->y();
    float r = e->x() - w;
    Vector3 up = scene->camera.up.normalize()*u;
    Vector3 right = forw.cross(scene->camera.up).normalize()*r;
    return scene->camera.position + forw + up + right;
}

void MyGLWidget::resizeGL(int w, int h) {
    aspect = w/(float)h;
    glViewport(0,0,w,h);
}

void MyGLWidget::wheelEvent(QWheelEvent *e) {
    scene->camera.goForward(-e->delta()/100.0f);
}

void MyGLWidget::makeLayer() {
    scene->makeLayer();
    pushScene();
    if (scene->activeNode)
        emit itemSelected(scene, scene->activeNode);
}

void MyGLWidget::animate() {
        updateGL();
}

void MyGLWidget::setLayer(int i) {
    if (i >= 0 && i < scene->boxNode->children.size())
        scene->activeNode = scene->boxNode->children[i];
}

void MyGLWidget::toggleVisibility() {
    if (scene->setSeaLevel) {
        scene->boxNode->waterNode->visible = !scene->boxNode->waterNode->visible;
        return;
    }

    BaseNode* node = scene->activeNode;
    if (node) {
        node->visible = !node->visible;

        bool wasSurface = false;
        foreach (BaseNode* c,scene->boxNode->children) {

            if (wasSurface) {
                c->visible = node->visible;
            }
            if (c==node) {
                wasSurface =true;
            }
        }
    }

}

void MyGLWidget::setColor( QColor c) {
    if(scene->activeNode) {
        QVector4D& color = scene->activeNode->diffuse;
        color.setX(c.red()/255.0);
        color.setY(c.green()/255.0);
        color.setZ(c.blue()/255.0);
        color.setW(c.alpha()/255.0);
        scene->activeNode->ambient = QVector4D(0,0,0,1);
    }
}

void MyGLWidget::editLayer() {
    scene->editLayer();
    pushScene();
}



void MyGLWidget::makeRiver() {
    SurfaceNode* sn = qobject_cast<SurfaceNode*>(scene->activeNode);
    if (sn != NULL) {
        sn->makeRiverNode();
    }
}

void MyGLWidget::makeRidge() {
    SurfaceNode* sn = qobject_cast<SurfaceNode*>(scene->activeNode);
    if (sn != NULL) {
        sn->makeRidgeNode();
    }
}

void MyGLWidget::save() {
    QVariantMap map = scene->toJson();

    std::cout << "json:" << std::endl;

    bool succes = true;
    QByteArray bytes = QtJson::Json::serialize(map, succes);
    if (succes) {
        std::cout<<bytes.data()<<std::endl;
    }else {
        std::cout << "error" << std::endl;
    }


    QString fileName = QFileDialog::getSaveFileName(this,
         tr("Save Sketch"), "", tr("Geosketch Files (*.json)"));
    if (!fileName.endsWith(".json"))
        fileName += ".json";
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << bytes.data();

}

void MyGLWidget::open() {
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Sketch"), "", tr("Geosketch Files (*.json)"));

    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    QString content = in.readAll();

    QVariantMap map = QtJson::Json::parse(content).toMap();

    scene->fromJson(map);

    pushScene();

}

void MyGLWidget::seaLevel() {
    scene->seaLevel();

}

void MyGLWidget::createDeposits() {
    float seaLevel = scene->boxNode->getSeaLevel();

    RiverNode * riverNode = qobject_cast<RiverNode *>(scene->activeNode);
    SurfaceNode * activeSurface = NULL;

    if (riverNode != NULL) {
        activeSurface = qobject_cast<SurfaceNode *>(riverNode->parent);
        if (activeSurface != NULL) {
            riverNode->createDeposit(seaLevel, *activeSurface);
        }
    } else {
        Deposit * dep = qobject_cast<Deposit *>(scene->activeNode);
        if (dep  != NULL) {
            dep->setDepositing(!dep->isDepositing());
        }
    }



}

