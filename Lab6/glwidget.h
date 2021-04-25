#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <iostream>
#include <QGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QtCore/qmath.h>
#include <math.h>

class GLWidget : public QGLWidget, public QOpenGLFunctions {
    Q_OBJECT
private:
    float size = 4.0f;
    float R = 4.0f;
    int pred[3] = {0, 0, 0};
    QMatrix4x4 matrix;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    QOpenGLShaderProgram *m_program;

    float x0 = 0;
    float y0 = 0;
    float z0 = 1;
    float a = 1;
    float b = 1;
    float c = 1;
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();

    void paintGL();

    void resizeGL(int w, int h);

//    void wheelEvent(QWheelEvent *event);
    void initShaders();

    void draw();

    void drawVLines(double y0, double y1, float r);


private
    slots:
            void getX(int);

    void getY(int);

    void getZ(int);

    void getSize(int);

    void getR(int);

};

#endif // GLWIDGET_H
