#-------------------------------------------------
#
# Project created by QtCreator 2018-05-24T13:04:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OGL
TEMPLATE = app
LIBS += -lGLU


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    mainwindow.cpp \
        widget.cpp \
    simpleobject3d.cpp \
    group3d.cpp \
    camera3d.cpp \
    figurebuilder.cpp

HEADERS += \
    mainwindow.h \
        widget.h \
    simpleobject3d.h \
    transformational.h \
    group3d.h \
    camera3d.h \
    figurebuilder.h

DISTFILES += \
    fshader.glsl \
    vshader.glsl \
    vshader.vsh \
    fshader.fsh

RESOURCES += \
    shaders.qrc \
    vshader.vsh \
    fshader.fsh \
    textures.qrc \
    fshader.glsl \
    vshader.glsl

FORMS += \
    mainwindow.ui
