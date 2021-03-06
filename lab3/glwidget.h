#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <iostream>
#include <QtOpenGL>
#include <QtOpenGL/QGLWidget>
//#include <QtOpenGL/qglu.h>
//#include <QtOpenGL/qgl.h>


#include <GL/glu.h>
#include <GL/gl.h>
#include <dialog.h>

class GLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:


    struct mapCoords{
        int x, y;
    };

    struct glCoords{
        float x, y;
    };

    GLWidget(QWidget *parent = NULL);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    int isMouseOnPoint(QPoint point);
    GLWidget::mapCoords fromGLCoordsToMap(float x, float y);
    GLWidget::glCoords fromMapCoordsToGL(int x, int y);
    void updateSplinePoints();

    std::vector<std::pair<float, float>> points = {
        {-0.9, 0.9},
        {-0.4, 0.5},
        {0.0, 0.0},
        {0.5, 0.4},
        {0.7, -0.7}
    };

    std::vector<std::pair<float, float>> spline_points;

    std::vector<float> h = {2.0, 1.0, 2.0, 2.0, 5.0};

    Dialog* dialog = nullptr;

    int draggablePoint = -1;
};

#endif  /* _GLWIDGET_H */
