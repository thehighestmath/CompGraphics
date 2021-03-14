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
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    int isMouseOnPoint(QPoint point);
    GLWidget::mapCoords fromGLCoordsToMap(float x, float y);
    GLWidget::glCoords fromMapCoordsToGL(int x, int y);
    void updateSplinePoints();
    void drawRetcangle(std::vector<int> data, float angle);

//    std::vector<std::pair<float, float>> points = {
//        {-0.9, 0.9},
//        {-0.4, 0.5},
//        {0.0, 0.0},
//        {0.5, 0.4},
//        {0.7, -0.7}
//    };

    // x y w h angle
    // assert 4 elements
    std::vector<std::pair<
        std::vector<int>, float
    >> initFrac = {
        {{220, 350, 200, 100}, 0},
        {{140, 300, 100, 50}, 30},
        {{400, 280, 80, 40}, -15},
    };

    int draggablePoint = -1;
    int fractalLevel = 1;
    double angle = 0.0;
};

#endif  /* _GLWIDGET_H */
