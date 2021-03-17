#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <iostream>
#include <QtOpenGL>
#include <QtOpenGL/QGLWidget>
//#include <QtOpenGL/qglu.h>
//#include <QtOpenGL/qgl.h>


#include <GL/glu.h>
#include <GL/gl.h>

class GLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    GLWidget(QWidget *parent = NULL);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif  /* _GLWIDGET_H */
