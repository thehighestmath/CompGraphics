#include <QtGui/QMouseEvent>
#include "glwidget.h"


GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
}
void GLWidget::initializeGL() {

}
void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glBegin(GL_LINE_STRIP);

//    gl_Position = projection * view * model * vec4(aPos, 1.0);
//    gl_PointSize = gl_Position.z;

    glEnd();
}

