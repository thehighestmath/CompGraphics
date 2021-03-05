#include <QtGui/QMouseEvent>
#include "glwidget.h"

template<typename T>
void printVector(std::vector<std::pair<T, T>> points) {
    for (const auto &point : points) {
        std::cout << "(" << point.first << ";" << point.second << ") ";
    }
    std::cout << std::endl;
}

// TODO:
int k = 5;
int n = 4;

int steps = 100;

std::vector<float> h = {2.0, 1.0, 2.0, 2.0, 5.0};


double _t(int i) {
    if (0 <= i && i < k) {
        return 0;
    }
    if (k <= i && i <= n) {
        return i - k + 1;
    }
    if (n < i && i <= n + k) {
        return n - k + 2;
    }
    std::cerr << "Unreacheble state" << std::endl;
    return 0;
}

double N(int i, int k, double u) {
    if (k == 1) {
        if (_t(i) <= u && u <= _t(i + 1)) {
            return 1;
        }
        return 0;
    }
    double num1 = ((u - _t(i)) * N(i, k - 1, u));
    double den1 = (_t(i + k - 1) - _t(i));
    double num2 = ((_t(i + k) - u) * N(i + 1, k - 1, u));
    double den2 = (_t(i + k) - _t(i + 1));
    double frac1, frac2;

    if (fabs(num1) <= 1e-6 && fabs(den1) <= 1e-6) {
        frac1 = 0;
    } else {
        frac1 = num1 / den1;
    }
    if (fabs(num2) <= 1e-6 && fabs(den2) <= 1e-6) {
        frac2 = 0;
    } else {
        frac2 = num2 / den2;
    }

    return frac1 + frac2;
}


double P(double u, std::vector<float> p) {
    double numerator = 0, denominator = 0;
    for (int i = 0; i < n + 1; i++) {
        double temp = h[i] * N(i, k, u);
        numerator += temp * p[i];
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
        x = P(t, xPoints);
        y = P(t, yPoints);
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

    glLineWidth(5.0f);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (const auto &point : points) {
        glVertex2f(point.first, point.second);
    }
    glEnd();

//    glPointSize(5.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
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
