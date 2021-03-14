#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    glWidget = new GLWidget();
    glWidget->initializeGL();
    ui->gridLayout->addWidget(glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    std::cout << arg1 << std::endl;
    glWidget->fractalLevel = arg1;
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    glWidget->angle = arg1;
    glWidget->update();
}
