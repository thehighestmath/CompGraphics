//#include "widget.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QSurfaceFormat format;
//    format.setSamples(16);
//    format.setDepthBufferSize(24);
//    QSurfaceFormat::setDefaultFormat(format);

//    Widget w;
//    w.show();
    MainWindow w;
    w.show();

    return a.exec();
}
