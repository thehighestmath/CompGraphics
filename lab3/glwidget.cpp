#include <QtGui/QMouseEvent>
#include "glwidget.h"

template<typename T>
void printVector(std::vector<std::pair<T, T>> points){
    for (const auto &point : points) {
        std::cout << "(" << point.first << ";" << point.second << ") ";
    }
    std::cout << std::endl;
}


int _i = 1;
int _m = 5;
int steps = 20;
std::vector<float> w = {2.0, 1.0, 2.0, 1.0, 5.0};

double _t(int i){
    return 1.0 * i / steps;
}

double M(int i, double t) {
    if (_t(i) <= t && t < _t(i + 1)){
        return _t(i + 1) - _t(i);
    }
//    return 1.0 * steps;
    return 0;
}

double M(int i, int m, double t) {
    if (m == 1) {
        return M(i, t);
    }
    return ((_t(i + m) - t) * M(i + 1, m - 1, t) +
            (t - _t(i)) * M(i, m - 1, t)) / (_t(m));
}

double N(int j, int m, double t) {
    return (_t(j + m) - _t(j)) * M(j, m, t);
}

double r(double t, std::vector<float>& p, std::vector<float>& w) {
    double numerator = 0, denominator = 0;
    for (int j = _i - _m + 1; j <= _i; j++) {
        double temp = N(j, _m, t) * w[j + 3];
        numerator += temp * p[j + 3];
        denominator += temp;
    }
//    std::cout << numerator << " "<< denominator << std::endl;
    return numerator / denominator;
}



void GLWidget::updateSplinePoints() {
    spline_points.clear();
    std::vector<float> xPoints;
    std::vector<float> yPoints;
    for (const auto &point : points) {
        xPoints.push_back(point.first);
        yPoints.push_back(point.second);
    }
    for (int i = 0; i < steps + 1; i++){
        double x, y;
        double t = i / static_cast<float>(steps);
//        std::cout << t << std::endl;
        x = r(t, xPoints, w);
        y = r(t, yPoints, w);
        spline_points.push_back({x, y});
//        std::cout << "count of elements is " << spline_points.size() << std:: endl;
    }
//    printVector(spline_points);
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
//    std::cout << index << std::endl;
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
