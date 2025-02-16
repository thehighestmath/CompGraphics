#ifndef FIGUREBUILDER_H
#define FIGUREBUILDER_H

#include "simpleobject3d.h"
#include <iostream>

class FigureBuilder
{
public:
    FigureBuilder();

    static SimpleObject3D *initCube(const QImage &texture, float width);
    static SimpleObject3D *initDiskSector(const QImage &texture, QVector3D point = QVector3D(0.0f, 0.0f, 0.0f), double r = 1.0, double angle = M_PI / 2, double step = 0.1, bool invert = false);
    static SimpleObject3D *initBelt(const QImage &texture, QVector3D center1, QVector3D center2, double r1, double r2, double step = 0.1);
    static SimpleObject3D *initCylinder(QVector3D center1, QVector3D center2, double r1, double r2, double step);
    static double calculteLemniscatePoint(double x, double c);
    static SimpleObject3D *initParallelepiped(const QImage &texture, QVector3D centerTop, double width, double height, double z);
    static SimpleObject3D *initSquareBelt(const QImage &texture, QVector3D center1, double w1, double h1, QVector3D center2, double w2, double h2);
    static SimpleObject3D *initStair(const QImage &texture, QVector3D center1, QVector3D center2, double r1, double r2, double step = 0.1);

    static SimpleObject3D *initLine(const QImage &texture, QVector3D center1, QVector3D center2, double r);

};

#endif // FIGUREBUILDER_H
