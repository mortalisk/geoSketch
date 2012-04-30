######################################################################
# Automatically generated by qmake (2.01a) Sun Mar 13 12:45:58 2011
######################################################################

TEMPLATE = app
TARGET = geoSketch
QT += core gui opengl
DEPENDPATH += . gui geometry morten3d
INCLUDEPATH += . gui geometry morten3d include
LIBS += -Llib -lGLEW

# Input
FORMS += gui/mainwindow.ui

# Input
HEADERS += gui/mainwindow.h \
    gui/sketchglwidget.h \
    geometry/shape.h \
    geometry/arrays.h \
    geometry/scene.h \
    morten3d/Camera.h \
    morten3d/Vector3.h \
    geometry/sphere.h \
    geometry/spline.h \
    geometry/surface.h \
    geometry/boxnode.h \
    geometry/sidenode.h \
    geometry/surfacenode.h \
    geometry/ridgenode.h \
    geometry/basenode.h \
    geometry/generalnode.h \
    geometry/isurfacefeature.h \
    geometry/util.h \
    gui/sketchpad.h \
    geometry/rivernode.h

SOURCES += main.cpp \
    gui/mainwindow.cpp \
    gui/sketchglwidget.cpp \
    geometry/shape.cpp\
    geometry/arrays.cpp \
    geometry/scene.cpp \
    morten3d/Camera.cpp \
    morten3d/Vector3.cpp \
    geometry/sphere.cpp \
    geometry/spline.cpp \
    geometry/surface.cpp \
    geometry/boxnode.cpp \
    geometry/sidenode.cpp \
    geometry/surfacenode.cpp \
    geometry/ridgenode.cpp \
    geometry/generalnode.cpp \
    geometry/basenode.cpp \
    geometry/isurfacefeature.cpp \
    gui/sketchpad.cpp \
    geometry/util.cpp \
    geometry/rivernode.cpp
