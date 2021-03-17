#include <QtGui/QMouseEvent>
#include "glwidget.h"

template<typename T>
void printVector(std::vector<std::pair<T, T>> points) {
    for (const auto &point : points) {
        std::cout << "(" << point.first << ";" << point.second << ") ";
    }
    std::cout << std::endl;
}


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

void GLWidget::drawRetcangle(std::vector<int> data, float angle){
    glCoords p1 = fromMapCoordsToGL(data[0], data[1]);
    glCoords p2 = fromMapCoordsToGL(data[0] + data[2] * cos(angle * M_PI / 180), data[1] - data[2] * sin(angle * M_PI / 180));
    glCoords p3 = fromMapCoordsToGL(data[0] + data[2] * cos(angle * M_PI / 180) + data[3] * sin(angle * M_PI / 180), data[1] - data[2] * sin(angle * M_PI / 180) + data[3] * cos(angle * M_PI / 180));
    glCoords p4 = fromMapCoordsToGL(data[0] + data[3] * sin(angle * M_PI / 180), data[1] + data[3] * cos(angle * M_PI / 180));
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glVertex2f(p3.x, p3.y);
        glVertex2f(p4.x, p4.y);
    glEnd();
}

void GLWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(2.0f);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    for (const auto& x : initFrac){
        drawRetcangle(x.first, x.second);
    }
//    glPointSize(5.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 1.0f);

    glEnd();

    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f);

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
//    for (const auto &point : points) {
//        mapCoords coords = fromGLCoordsToMap(point.first, point.second);
//        if (abs(x - coords.x) < hitBox && abs(y - coords.y) < hitBox) {
//            return index;
//        }
//        index++;
//    }
    return -1;
}
void GLWidget::mousePressEvent(QMouseEvent *event) {
//    printf("%d, %d\n", event->x(), event->y());
    int index = isMouseOnPoint(event->pos());
//    std::cout << index << std::endl;
    draggablePoint = index;
}
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
//    printf("%d, %d\n", event->x(), event->y());
    if (draggablePoint != -1){
//        glCoords coords = fromMapCoordsToGL(x, y);
//        points[draggablePoint].first = coords.x;
//        points[draggablePoint].second = coords.y;
//        update();
    }

}
void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
//    printf("%d, %d\n", event->x(), event->y());
    draggablePoint = -1;
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    int index = isMouseOnPoint(event->pos());
//    std::cout << index << std::endl;
    draggablePoint = index;
}
