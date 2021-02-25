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

double bezier(double A,  // Start value
              double B,  // First control value
              double C,  // Second control value
              double D,  // Ending value
              double t)  // Parameter 0 <= t <= 1
{
    double s = 1 - t;
    double AB = A*s + B*t;
    double BC = B*s + C*t;
    double CD = C*s + D*t;
    double ABC = AB*s + BC*t;
    double BCD = BC*s + CD*t;
    return ABC*s + BCD*t;
}

void GLWidget::updateSplinePoints() {
    spline_points.clear();
    int steps = 100;
    for (int i = 0; i < steps + 1; i++){
        double x = bezier(
                points[0].first,
                points[1].first,
                points[2].first,
                points[3].first,
                i / static_cast<float>(steps)
                );
        double y = bezier(
                points[0].second,
                points[1].second,
                points[2].second,
                points[3].second,
                i / static_cast<float>(steps)
                );
        spline_points.push_back({x, y});
    }
}

void GLWidget::paintGL() {
    updateSplinePoints();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(3.0f);
    glEnable(GL_POINT_SMOOTH);

    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (const auto &point : points) {
        glVertex2f(point.first, point.second);
    }
    glEnd();

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 1.0f);
    for (const auto &point : spline_points) {
        glVertex2f(point.first, point.second);
    }
    glEnd();

    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (const auto &point : points) {
        glVertex2f(point.first, point.second);
    }
    glEnd();
}

GLWidget::mapCoords GLWidget::fromGLCoordsToMap(float x, float y){
    ++x /= 2;
    --y /= (-2);
    mapCoords coords = {
        static_cast<int>(this->width() * x),
        static_cast<int>(this->height() * y)
    };
    return coords;
}

GLWidget::glCoords GLWidget::fromMapCoordsToGL(int x, int y){
    float x_100 = 1.0 * x / this->width() * (2) - 1;
    float y_100 = 1.0 * y / this->height() * (-2) + 1;
    glCoords coords = {x_100, y_100};
    return coords;
}

int GLWidget::isMouseOnPoint(QPoint point){
    int x = point.x();
    int y = point.y();
    int hitBox = 10; // px
    int index = 0;
    for (const auto &point : points) {
        mapCoords coords = fromGLCoordsToMap(point.first, point.second);
        if (abs(x - coords.x) < hitBox && abs(y - coords.y) < hitBox) {
            return index;
        }
        index++;
    }
    return -1;
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
//    printf("%d, %d\n", event->x(), event->y());
    int index = isMouseOnPoint(event->pos());
    std::cout << index << std::endl;
    draggablePoint = index;
}
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
//    printf("%d, %d\n", event->x(), event->y());
    if (draggablePoint != -1){
        glCoords coords = fromMapCoordsToGL(x, y);
        points[draggablePoint].first = coords.x;
        points[draggablePoint].second = coords.y;
        update();
    }

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
//    printf("%d, %d\n", event->x(), event->y());
    draggablePoint = -1;
}
