#ifndef WIDGET_H
#define WIDGET_H

//#include <random>

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QVector>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QBasicTimer>
#include <QtMath>
#include <QKeyEvent>
#include <iostream>

#include "simpleobject3d.h"
#include "group3d.h"
#include "camera3d.h"
#include "figurebuilder.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Widget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void draw();
    void initShaders();
    void initHyperboloid(QVector3D center, double R, double delta = 0.1);
    void initScene();

    void initSandGlass();
    void initWoodenWheel();
    void initSandGlass2(double lowerBound = -1.0, double upperBound = 1.0, double c = 1.0, double delta = 0.1);
    void initBook(QVector3D centerTop, QVector3D centerBottom, double sideX, double sideY, double delta = 0.1);
    void initSimpleBook(const QImage &texture, QVector3D centerTop, QVector3D centerBottom, double sideX, double sideY, double delta = 0.1);
    void initStairs(QVector3D centerTop, QVector3D centerBottom, double width, double delta = 0.1);
    void initStairs(QVector3D centerTop, double width, double height, double side, double delta, double angle);
    void initBarrel(double leftOffset, double delta);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    // матрица проекции
    QMatrix4x4 m_projectionMatrix;

    // для подключения шейдеров
    QOpenGLShaderProgram* m_program;

    // все группы объектов на сцене
    QVector< QSharedPointer<Group3D> > m_groups;

    // все корневые группы объектов на сцене
    QVector< QSharedPointer<Transformational> > m_transformObjects;

    // для вращения сцены
    QVector2D m_mousePosition;

    // для анимации
    QBasicTimer m_timer;
    float m_angleMain = 0.0f;

    QSharedPointer<Camera3D> m_camera;
    QVector4D m_lightPosition;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    float x0 = 0;
    float y0 = 0;
    float z0 = 1;
    float a = 1;
    float b = 1;
    float c = 1;
    float size = 4.0f;
    float R = 4.0f;
    int pred[3] = {0, 0, 0};
    int countSteps;
    bool proection = false;

private
    slots:
    void getX(int);

    void getY(int);

    void getZ(int);

    void getSize(int);

    void getR(int);

    void changeProection();

};

#endif // WIDGET_H
