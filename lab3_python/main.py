import sys
from OpenGL.GL import *
from OpenGL.GLU import *
from PyQt5 import QtGui
from PyQt5.QtOpenGL import *
from PyQt5 import QtCore, QtWidgets, QtOpenGL

POINT_SIZE = 10
nurb = None
samplingTolerance = 1.0


class Ui_MainWindow(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(Ui_MainWindow, self).__init__()
        self.widget = glWidget()
        self.button = QtWidgets.QPushButton('Test', self)
        mainLayout = QtWidgets.QHBoxLayout()
        mainLayout.addWidget(self.widget)
        mainLayout.addWidget(self.button)
        self.setLayout(mainLayout)


class glWidget(QtWidgets.QOpenGLWidget):
    def __init__(self, parent=None):
        QtWidgets.QOpenGLWidget.__init__(self, parent)
        self.setMinimumSize(640, 480)
        self.setMouseTracking(True)
        self.points = [
            [50 * i + 10, 10 * i + 20, 0] for i in range(5)
        ]
        degree = 3
        for i in range(degree - 1):
            self.points = self.points + [self.points[i]]
        knotNum = len(self.points) + degree
        self.knots = [float(i) / (knotNum - 1) for i in range(knotNum)]
        self.activePoint = -1

        glClearColor(0, 0, 0, 0)
        glShadeModel(GL_FLAT)
        global nurb
        nurb = gluNewNurbsRenderer()
        global samplingTolerance
        glLineWidth(2.0)
        gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, samplingTolerance)

    def paintGL(self):
        width = self.width()
        height = self.height()
        glViewport(0, 0, width, height)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluOrtho2D(0, width, 0, height)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

        glEnable(GL_POINT_SMOOTH)
        glPointSize(POINT_SIZE)

        glBegin(GL_POINTS)
        for i in self.points:
            glColor3f(1.0, 1.5, 0.0)
            glVertex2f(i[0], height - i[1])
        glEnd()

        glLineWidth(2)
        glBegin(GL_LINE_STRIP)
        for i in self.points:
            glColor3f(1, 1, 0)
            glVertex2f(i[0], height - i[1])
        glEnd()
        glDisable(GL_POINT_SMOOTH)

        global nurb
        gluBeginCurve(nurb)
        print(self.knots, self.points)
        gluNurbsCurve(nurb, self.knots, self.points, GL_MAP1_VERTEX_3)
        gluEndCurve(nurb)

        glFlush()

    def initializeGL(self):
        glClearDepth(1.0)
        glClearColor(0, 0, 0, 1)
        glDepthFunc(GL_LESS)
        glEnable(GL_DEPTH_TEST)
        glShadeModel(GL_SMOOTH)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glEnable(GL_POINT_SMOOTH)
        gluPerspective(45.0, 1.33, 0.1, 100.0)
        glMatrixMode(GL_MODELVIEW)

    def mousePressEvent(self, a0: QtGui.QMouseEvent) -> None:
        self.activePoint = -1
        for i in self.points:
            if abs(a0.x() - i[0]) < POINT_SIZE and abs(a0.y() - i[1]) < POINT_SIZE:
                self.activePoint = self.points.index(i)
                break
        self.update()

    def mouseMoveEvent(self, a0: QtGui.QMouseEvent) -> None:
        if self.activePoint != -1:
            self.points[self.activePoint] = [a0.x(), a0.y(), self.points[self.activePoint][2]]
            self.update()

    def mouseReleaseEvent(self, a0: QtGui.QMouseEvent) -> None:
        self.activePoint = -1
        self.update()


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    Form = QtWidgets.QMainWindow()
    ui = Ui_MainWindow(Form)
    ui.show()
    sys.exit(app.exec_())
