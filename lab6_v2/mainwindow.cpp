#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Lab6");
    setFixedSize(900, 600);
    glWidget = new Widget(this);
    sliderX = new QSlider(Qt::Horizontal, this);
    sliderX->setGeometry(600, 70, 250, 10);
    sliderX->setAutoFillBackground(true);
    sliderX->setMinimum(0);
    sliderX->setMaximum(100);
    sliderX->setValue(0);
    sliderX->setTracking(true);
    labelX = new QLabel("X", this);
    labelX->setGeometry(610, 50, 25, 10);

    sliderY = new QSlider(Qt::Horizontal, this);
    sliderY->setGeometry(600, 110, 250, 10);
    sliderY->setAutoFillBackground(true);
    sliderY->setMinimum(0);
    sliderY->setMaximum(100);
    sliderY->setValue(0);
    sliderY->setTracking(true);
    labelY = new QLabel("Y", this);
    labelY->setGeometry(610, 90, 25, 10);

    sliderZ = new QSlider(Qt::Horizontal, this);
    sliderZ->setGeometry(600, 150, 250, 10);
    sliderZ->setAutoFillBackground(true);
    sliderZ->setMinimum(0);
    sliderZ->setMaximum(100);
    sliderZ->setValue(0);
    sliderZ->setTracking(true);
    labelZ = new QLabel("Z", this);
    labelZ->setGeometry(610, 130, 25, 10);

    sliderSize = new QSlider(Qt::Horizontal, this);
    sliderSize->setGeometry(600, 190, 250, 10);
    sliderSize->setAutoFillBackground(true);
    sliderSize->setMinimum(1);
    sliderSize->setMaximum(7);
    sliderSize->setValue(4);
    sliderSize->setTracking(true);
    labelSize = new QLabel("Size", this);
    labelSize->setGeometry(610, 170, 25, 10);

    sliderR = new QSlider(Qt::Horizontal, this);
    sliderR->setGeometry(600, 230, 250, 10);
    sliderR->setAutoFillBackground(true);
    sliderR->setMinimum(1);
    sliderR->setMaximum(10);
    sliderR->setValue(4);
    sliderR->setTracking(true);
    labelR = new QLabel("R", this);
    labelR->setGeometry(610, 210, 25, 10);

    QObject::connect(sliderX, SIGNAL(valueChanged(int)), glWidget, SLOT(getX(int)));
    QObject::connect(sliderY, SIGNAL(valueChanged(int)), glWidget, SLOT(getY(int)));
    QObject::connect(sliderZ, SIGNAL(valueChanged(int)), glWidget, SLOT(getZ(int)));
    QObject::connect(sliderSize, SIGNAL(valueChanged(int)), glWidget, SLOT(getSize(int)));
    QObject::connect(sliderR, SIGNAL(valueChanged(int)), glWidget, SLOT(getR(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
