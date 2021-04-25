#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <glwidget.h>
#include <QSlider>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    GLWidget *glWidget;
    QLabel *labelX;
    QLabel *labelY;
    QLabel *labelZ;
    QLabel *labelSize;
    QLabel *labelR;
    QSlider *sliderX;
    QSlider *sliderY;
    QSlider *sliderZ;
    QSlider *sliderSize;
    QSlider *sliderR;
};

#endif // MAINWINDOW_H
