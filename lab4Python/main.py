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
        self.spinbox1.setRange(0, 360)

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
        glBegin(GL_LINE_LOOP)
        glVertex2f((rect['x'] + rect['w']) * cos(rect['a']) - (rect['y'] + rect['h']) * sin(rect['a']),
                   (rect['x'] + rect['w']) * sin(rect['a']) + (rect['y'] + rect['h']) * cos(rect['a']))
        glVertex2f((rect['x'] - rect['w']) * cos(rect['a']) - (rect['y'] + rect['h']) * sin(rect['a']),
                   (rect['x'] - rect['w']) * sin(rect['a']) + (rect['y'] + rect['h']) * cos(rect['a']))
        glVertex2f((rect['x'] - rect['w']) * cos(rect['a']) - (rect['y'] - rect['h']) * sin(rect['a']),
                   (rect['x'] - rect['w']) * sin(rect['a']) + (rect['y'] - rect['h']) * cos(rect['a']))
        glVertex2f((rect['x'] + rect['w']) * cos(rect['a']) - (rect['y'] - rect['h']) * sin(rect['a']),
                   (rect['x'] + rect['w']) * sin(rect['a']) + (rect['y'] - rect['h']) * cos(rect['a']))

        # glVertex2f(rect['x'] - rect['w'] * cos(rect['a']) - rect['y'] + rect['h'] * sin(rect['a']),
        #            rect['x'] - rect['w'] * sin(rect['a']) + rect['y'] - rect['h'] * cos(rect['a']))
        # glVertex2f(rect['x'] + rect['w'] * cos(rect['a']) - rect['y'] + rect['h'] * sin(rect['a']),
        #            rect['x'] + rect['w'] * sin(rect['a']) + rect['y'] - rect['h'] * cos(rect['a']))



        # glVertex2f((rect['x'] + rect['w']) * cos(rect['a']) - (rect['y'] + rect['h']) * sin(rect['a']),
        #            (rect['x'] + rect['w']) * sin(rect['a']) + (rect['y'] + rect['h']) * cos(rect['a']))

        # glVertex2f(rect['x'] + rect['w'] * cos(rect['a']) - rect['y'] - rect['h'] * sin(rect['a']),
        #            (rect['x'] + rect['w'] * sin(rect['a']) + rect['y'] + rect['h'] * cos(rect['a'])))
        # glVertex2f(rect['x'] - rect['w'] * cos(rect['a']) - rect['y'] - rect['h'] * sin(rect['a']),
        #            (rect['x'] - rect['w'] * sin(rect['a']) + rect['y'] + rect['h'] * cos(rect['a'])))
        #
        # glVertex2f(rect['x'] - rect['w'] * cos(rect['a']) - rect['y'] + rect['h'] * sin(rect['a']),
        #            rect['x'] - rect['w'] * sin(rect['a']) + rect['y'] - rect['h'] * cos(rect['a']))
        # glVertex2f(rect['x'] + rect['w'] * cos(rect['a']) - rect['y'] + rect['h'] * sin(rect['a']),
        #            rect['x'] + rect['w'] * sin(rect['a']) + rect['y'] - rect['h'] * cos(rect['a']))
        # glVertex2f(rect['x'] + rect['w'] * cos(rect['a']) - rect['y'] - rect['h'] * sin(rect['a']),
        #            rect['x'] + rect['w'] * sin(rect['a']) + rect['y'] + rect['h'] * cos(rect['a']))


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

        # glPointSize(POINT_SIZE)

        # glBegin(GL_POINTS)
        # for i in self.points:
        #     glColor3f(1.0, 1.0, 0.0)
        #     glVertex2f(i[0], height - i[1])
        # glEnd()
        #
        # glLineWidth(2)
        # glBegin(GL_LINE_STRIP)
        # for i in self.points:
        #     glColor3f(1, 0.5, 0)
        #     glVertex2f(i[0], height - i[1])
        # glEnd()
        # glDisable(GL_POINT_SMOOTH)
        #
        # # global nurb
        # glColor3f(0, 0.5, 1)
        # glLineWidth(4)
        # glBegin(GL_LINES)
        # k = 4
        # for i in range(1, len(self.points)):
        #     # x points
        #     point = []
        #     point.append(P(self.points[i][0], n, k, [j[0] for j in self.points], [j[2] for j in self.points]))
        #     point.append(P(self.points[i][1], n, k, [j[0] for j in self.points], [j[2] for j in self.points]))
        #     if point[0] != 0 and point[1] != 0:
        #         print("i = {} point = {}\n-------------------".format(i, point))
        #     glVertex2f(point[0], point[1])
        #     # for j in range(self.points[i][0], self.points[i + 1][0]):
        #     # array.append([P()])
        #     # pass
        # glEnd()
        # gluBeginCurve(nurb)
        # print(self.knots, self.points)
        # coordinate = [[self.points[i][0], height - self.points[i][1], self.points[i][2], self.points[i][3]] for i in
        #               range(len(self.points))]
        # gluNurbsCurve(nurb, self.knots, coordinate, GL_MAP1_VERTEX_3)
        # gluEndCurve(nurb)

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
