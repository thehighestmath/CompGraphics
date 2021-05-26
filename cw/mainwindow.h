#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <widget.h>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;

    Widget *glWidget;
    QLabel *labelX;
    QLabel *labelY;
    QLabel *labelZ;
    QLabel *labelSize;
    QLabel *labelR;
    QSlider *sliderX;
    QSlider *sliderY;
    QSlider *sliderZ;

    QSlider *sliderXAmbient;
    QSlider *sliderYAmbient;
    QSlider *sliderZAmbient;
    QSlider *sliderAmbient;

    QSlider *sliderSize;
    QSlider *sliderR;
    QCheckBox *check;
};

#endif // MAINWINDOW_H
