#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Кибардин, Крыжановский. 8303.");
    setFixedSize(900, 600);
    glWidget = new Widget(this);
//    sliderX = new QSlider(Qt::Horizontal, this);
//    sliderX->setGeometry(600, 70, 250, 10);
//    sliderX->setAutoFillBackground(true);
//    sliderX->setMinimum(0);
//    sliderX->setMaximum(100);
//    sliderX->setValue(0);
//    sliderX->setTracking(true);
//    labelX = new QLabel("X", this);
//    labelX->setGeometry(610, 50, 25, 10);

//    sliderY = new QSlider(Qt::Horizontal, this);
//    sliderY->setGeometry(600, 110, 250, 10);
//    sliderY->setAutoFillBackground(true);
//    sliderY->setMinimum(0);
//    sliderY->setMaximum(100);
//    sliderY->setValue(0);
//    sliderY->setTracking(true);
//    labelY = new QLabel("Y", this);
//    labelY->setGeometry(610, 90, 25, 10);

//    sliderZ = new QSlider(Qt::Horizontal, this);
//    sliderZ->setGeometry(600, 150, 250, 10);
//    sliderZ->setAutoFillBackground(true);
//    sliderZ->setMinimum(0);
//    sliderZ->setMaximum(100);
//    sliderZ->setValue(0);
//    sliderZ->setTracking(true);
//    labelZ = new QLabel("Z", this);
//    labelZ->setGeometry(610, 130, 25, 10);

//    sliderSize = new QSlider(Qt::Horizontal, this);
//    sliderSize->setGeometry(600, 190, 250, 10);
//    sliderSize->setAutoFillBackground(true);
//    sliderSize->setMinimum(1);
//    sliderSize->setMaximum(7);
//    sliderSize->setValue(4);
//    sliderSize->setTracking(true);
//    labelSize = new QLabel("Size", this);
//    labelSize->setGeometry(610, 170, 25, 10);

//    sliderR = new QSlider(Qt::Horizontal, this);
//    sliderR->setGeometry(600, 230, 250, 10);
//    sliderR->setAutoFillBackground(true);
//    sliderR->setMinimum(1);
//    sliderR->setMaximum(10);
//    sliderR->setValue(4);
//    sliderR->setTracking(true);
//    labelR = new QLabel("R", this);
//    labelR->setGeometry(610, 210, 25, 10);

    check = new QCheckBox(this);
    check->setText("setOrtho");
    check->setGeometry(620, 250, 150, 30);

    QLabel *x, *y, *z, *a;
//    x

    x = new QLabel(this);
    y = new QLabel(this);
    z = new QLabel(this);
    a = new QLabel(this);

    this->sliderXAmbient = new QSlider(Qt::Horizontal, this);
    this->sliderYAmbient = new QSlider(Qt::Horizontal, this);
    this->sliderZAmbient = new QSlider(Qt::Horizontal, this);
    this->sliderAmbient  = new QSlider(Qt::Horizontal, this);

    x->setText("x");
    y->setText("y");
    z->setText("z");
    a->setText("a");

    x->setGeometry(600, 100, 30, 30);
    y->setGeometry(600, 120, 30, 30) ;
    z->setGeometry(600, 140, 30, 30);
    a->setGeometry(600, 160, 30, 30);

    this->sliderXAmbient->setGeometry(620, 100, 150, 30);
    this->sliderYAmbient->setGeometry(620, 120, 150, 30) ;
    this->sliderZAmbient->setGeometry(620, 140, 150, 30);
    this->sliderAmbient->setGeometry(620, 160, 150, 30);

    this->sliderXAmbient->setMinimum(-100);
    this->sliderXAmbient->setMaximum(100);
    this->sliderYAmbient->setMinimum(-100);
    this->sliderYAmbient->setMaximum(100);
    this->sliderZAmbient->setMinimum(-100);
    this->sliderZAmbient->setMaximum(100);
    this->sliderZAmbient->setMinimum(-100);
    this->sliderZAmbient->setMaximum(100);

    this->sliderAmbient->setMinimum(0);
    this->sliderAmbient->setMaximum(200);

    this->sliderXAmbient->setValue(0);
    this->sliderYAmbient->setValue(0);
    this->sliderZAmbient->setValue(0);
    this->sliderAmbient->setValue(50);


//    QObject::connect(sliderX, SIGNAL(valueChanged(int)), glWidget, SLOT(getX(int)));
//    QObject::connect(sliderY, SIGNAL(valueChanged(int)), glWidget, SLOT(getY(int)));
//    QObject::connect(sliderZ, SIGNAL(valueChanged(int)), glWidget, SLOT(getZ(int)));
//    QObject::connect(sliderSize, SIGNAL(valueChanged(int)), glWidget, SLOT(getSize(int)));
//    QObject::connect(sliderR, SIGNAL(valueChanged(int)), glWidget, SLOT(getR(int)));
    QObject::connect(check, SIGNAL(stateChanged(int)), glWidget, SLOT(changeProection()));
    QObject::connect(sliderXAmbient, SIGNAL(valueChanged(int)), glWidget, SLOT(setLightX(int)));
    QObject::connect(sliderYAmbient, SIGNAL(valueChanged(int)), glWidget, SLOT(setLightY(int)));
    QObject::connect(sliderZAmbient, SIGNAL(valueChanged(int)), glWidget, SLOT(setLightZ(int)));
    QObject::connect(sliderAmbient , SIGNAL(valueChanged(int)), glWidget, SLOT(setLightAmbient(int)));
//    QObject::connect(this->ui->radioButton, SIGNAL(clicked()), glWidget, SLOT(changeProection()));
//    QObject::connect(this->ui->radioButton_2, SIGNAL(clicked()), glWidget, SLOT(changeProection()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::horizontalSlider_valueChanged(int value)
//{
////    this->glWidget
//    this->glWidget->setLightX(value);
//}

//void MainWindow::horizontalSlider_y_valueChanged(int value)
//{
//    this->glWidget->setLightY(value);
//}

//void MainWindow::horizontalSlider_z_valueChanged(int value)
//{
//    this->glWidget->setLightY(value);
//}

//void MainWindow::horizontalSlider_z_2_valueChanged(int value)
//{
//    this->glWidget->setLightAmbient(value);
//}
