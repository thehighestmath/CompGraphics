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
