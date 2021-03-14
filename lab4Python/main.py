import sys
import random

from OpenGL.GL import *
from OpenGL.GLU import *
from PyQt5 import QtGui
from PyQt5.QtOpenGL import *
from PyQt5 import QtCore, QtWidgets, QtOpenGL
import PyQt5
from PyQt5.QtCore import pyqtSlot
from numpy.ma import cos, sin
import numpy


class Ui_MainWindow(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(Ui_MainWindow, self).__init__()
        self.widget = glWidget()
        self.spinbox = PyQt5.QtWidgets.QSpinBox(self)
        self.spinbox.setRange(0, 5)
        self.spinbox1 = PyQt5.QtWidgets.QSpinBox(self)
        self.spinbox1.setRange(-360, 360)

        main_layout = QtWidgets.QHBoxLayout()
        main_layout.addWidget(self.widget)
        main_layout.addWidget(self.spinbox)

        self.setLayout(main_layout)
        self.connect_signals()
        self.setFixedSize(800, 600)

    @pyqtSlot(int)
    def f_rep_changed(self, value):
        print(value)

    @pyqtSlot(int)
    def f_rep_changed1(self, value):
        self.widget.spin2(value)

    def connect_signals(self):
        self.spinbox.valueChanged.connect(self.f_rep_changed)
        self.spinbox1.valueChanged.connect(self.f_rep_changed1)


class glWidget(QtWidgets.QOpenGLWidget):
    def __init__(self, parent=None):
        QtWidgets.QOpenGLWidget.__init__(self, parent)
        self.setFixedSize(640, 480)
        self.setMouseTracking(True)

        self.a = 0
        glClearColor(0, 0, 0, 0)
        glShadeModel(GL_FLAT)

    def spinBoxChanged(self, val):
        self.fractalDepth = val
        print(val)
        self.update()

    def spin2(self, val):
        self.a = val
        print(val)
        self.update()

    def getCoordinate(self, coordinate):
        r = random.random()
        s = random.random()
        h = random.random()
        k = random.random()
        phi = numpy.pi / 8 + random.uniform(-0.1, 0.1)
        theta = numpy.pi / 8 + random.uniform(-0.1, 0.1)
        x = r * cos(theta) * coordinate.x + s * sin(phi) * coordinate.y + h
        y = -r * sin(theta) * coordinate.x + s * cos(phi) * coordinate.y + k
        return x, y

    def paint_rectangle(self, rect):
        x = rect['x']
        y = rect['y']
        w = rect['w']
        h = rect['h']
        a = rect['a']
        glBegin(GL_LINE_LOOP)
        glVertex2f(x + w * cos(a) - h * sin(a), y + w * sin(a) + h * cos(a))
        glVertex2f(x + w * cos(a) + h * sin(a), y + w * sin(a) - h * cos(a))
        glVertex2f(x - w * cos(a) + h * sin(a), y - w * sin(a) - h * cos(a))
        glVertex2f(x - w * cos(a) - h * sin(a), y - w * sin(a) + h * cos(a))

        glEnd()

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

        self.paint_rectangle({'x': 400, 'y': 240, 'h': 50, 'w': 100, 'a': numpy.pi * self.a / 180})
        
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


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    Form = QtWidgets.QMainWindow()
    ui = Ui_MainWindow(Form)
    ui.show()
    sys.exit(app.exec_())
