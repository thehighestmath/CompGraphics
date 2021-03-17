#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <glwidget.h>
#include <iostream>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;
};
#endif // MAINWINDOW_H
