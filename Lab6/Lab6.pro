#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T22:16:05
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = Lab6
TEMPLATE = app

LIBS += -lGLU

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    shader.qrc
