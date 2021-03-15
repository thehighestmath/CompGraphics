import copy
import random
import sys

import PyQt5
import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *
from PyQt5 import QtWidgets
from PyQt5.QtCore import pyqtSlot
from numpy.ma import cos, sin

ZERO_LEVEL = 200


class Ui_MainWindow(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(Ui_MainWindow, self).__init__()
        self.widget = GlWidget()
        self.spinbox = PyQt5.QtWidgets.QSpinBox(self)
        self.spinbox.setRange(0, 5)
        self.spinbox1 = PyQt5.QtWidgets.QSpinBox(self)
        self.spinbox1.setRange(-360, 360)

        main_layout = QtWidgets.QHBoxLayout()
        main_layout.addWidget(self.widget)
        main_layout.addWidget(self.spinbox)

        self.setLayout(main_layout)
        self.connect_signals()
        self.setFixedSize(800, 800)

    @pyqtSlot(int)
    def f_rep_changed(self, value):
        self.widget.spin_box_changed(value)
        pass

    @pyqtSlot(int)
    def f_rep_changed1(self, value):
        self.widget.spin2(value)

    def connect_signals(self):
        self.spinbox.valueChanged.connect(self.f_rep_changed)
        self.spinbox1.valueChanged.connect(self.f_rep_changed1)


class GlWidget(QtWidgets.QOpenGLWidget):
    def __init__(self, parent=None):
        QtWidgets.QOpenGLWidget.__init__(self, parent)
        self.setFixedSize(640, 640)
        self.setMouseTracking(True)

        self.a = 0
        self.fractal_depth = 0
        self.old_rects = [
            self.create_rectangle(self.width() / 2, self.height() / 2, 0, ZERO_LEVEL, ZERO_LEVEL)
        ]
        self.new_rects = []
        self.steps = {0: self.old_rects}

        glClearColor(0, 0, 0, 0)
        glShadeModel(GL_FLAT)

    def create_rectangle(self, x, y, angle, h, w):
        return {'x': x, 'y': y, 'h': h, 'w': w, 'a': angle}

    def spin_box_changed(self, val):
        self.fractal_depth = val
        self.update()

    def spin2(self, val):
        self.a = val
        self.update()

    # def get_coordinate(self, coordinate, hh, ww):
    #     r = 0.5
    #     s = 0.5
    #     h = 0.5
    #     k = 0.5
    #     x = coordinate['x']
    #     y = coordinate['y']
    #     phi = np.pi / 8 + random.uniform(-0.1, 0.1)
    #     theta = - np.pi / 8 + random.uniform(-0.1, 0.1)
    #     x = r * cos(theta) * x + s * sin(phi) * y + h
    #     y = -r * sin(theta) * x + s * cos(phi) * y + k
    #     return {'x': x + coordinate['x'] - ww / 2, 'y': y + coordinate['y'] - hh / 2}

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

    def generator(self):
        if self.fractal_depth in self.steps:
            self.old_rects = self.steps[self.fractal_depth]
            self.new_rects = []
            return
        # todo уменьшение размеров след квадратов
        for rect in self.old_rects:
            x = rect['x']
            y = rect['y']
            h = rect['h']
            w = rect['w']
            a = rect['a']

            # 3
            self.new_rects.append(self.create_rectangle(
                x - w + 2 * w * 72 / 240,
                y - h + 2 * h * (1 - 55 / 176),
                a + np.pi / 12,
                h * 67 / 176,
                w * 129 / 240
            ))

            # 2
            self.new_rects.append(self.create_rectangle(
                x - w + 2 * w * 177 / 240,
                y - h + 2 * h * (1 - 65 / 176),
                a + -np.pi / 8,
                h * 71 / 176,
                w * 102 / 240
            ))

            # 1
            self.new_rects.append(self.create_rectangle(
                x - w + 2 * w * 117 / 240,
                y - h + 2 * h * (1 - 120 / 176),
                a + 0,
                h * 117 / 176,
                w * 102 / 240
            ))
        self.old_rects = copy.deepcopy(self.new_rects)
        self.steps[self.fractal_depth] = copy.deepcopy(self.new_rects)
        self.new_rects = []

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

        # todo придумать отображение точек

        self.generator()
        for rect in self.old_rects:
            self.paint_rectangle(rect)

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
