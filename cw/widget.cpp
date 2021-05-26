#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_camera = QSharedPointer<Camera3D>(new Camera3D());
    m_camera->translate(QVector3D(0.0f, 5.0f, -5.0f));
    m_lightPosition = QVector4D(0.0, 0.0, 0.0, 1.0);
    setGeometry(50,50,500,500);

}

Widget::~Widget()
{
    //delete m_camera;
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    // каким цветом будет производиться очистка экрана
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    double precision = 0.1;
    QImage texture = QImage(":123.jpg");

    initShaders();
//    initSandGlass2(-1.0, 1.0, 0.9, precision);
//        initBook(QVector3D(0.0, 0.0, -1.4), QVector3D(0.0, 0.0, -2.0), 3, 4, 0.01);
//        initSimpleBook(texture, QVector3D(0.0, 0.0, -1.4), QVector3D(0.0, 0.0, -2.0), 3, 4, precision);

    initHyperboloid(QVector3D(0,0,0), R, 0.1);
    initScene();

}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / (float) h;
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.01f, 100.0f);
}

void Widget::paintGL()
{
    // выставляем флаги
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    m_program->bind();
    m_program->setUniformValue("u_projectionMatrix", m_projectionMatrix);
    drawTest();

    m_program->setUniformValue("u_lightPosition", QVector4D(this->xLight, this->yLight, this->zLight, 1.0));
    m_program->setUniformValue("u_lightPower", this->ambient/100);
    m_program->setUniformValue("u_ambident", 0.8f);

    m_camera->draw(m_program);

    for(auto object : m_transformObjects)
    {
        object->draw(m_program, context()->functions());
    }

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        m_mousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }

    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angle = diff.length() / 2.0;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);

    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
    {
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
    }
    else
    {
        if(event->delta() < 0)
        {
            m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
        }
    }
    update();
}

void Widget::timerEvent(QTimerEvent *event)
{

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    std::cout << event->key() <<std::endl;
    switch(event->key()) {
    case 87:
        m_camera->translate(QVector3D(0, 0, 1));
        break;
    case 83:
        m_camera->translate(QVector3D(0, 0, -1));
        break;

    case 65:
        m_camera->translate(QVector3D(1, 0, 0));
        break;
    case 68:
        m_camera->translate(QVector3D(-1, 0, 0));
        break;

    case 81:
        m_camera->translate(QVector3D(0, -1, 0));
        break;
    case 69:
        m_camera->translate(QVector3D(0, 1, 0));
        break;
    }
    this->update();
}

void Widget::initShaders()
{
    const GLubyte *extensions;
    extensions = glGetString(GL_EXTENSIONS);
    int i = 0;
    QString extensionsString;
    while (extensions[i] != 0) {
        extensionsString += extensions[i];
        i++;
    }
    if (!extensionsString.contains("GL_ARB_vertex_shader") ||
            !extensionsString.contains("GL_ARB_fragment_shader") ||
            !extensionsString.contains("GL_ARB_shader_objects") ||
            !extensionsString.contains("GL_ARB_shading_language_100"))
    {
        exit(0);
    }

    //compile shaders
    m_program = new QOpenGLShaderProgram( this );
    m_program->addShaderFromSourceFile( QOpenGLShader::Vertex, ":/vshader.vsh" );
    m_program->addShaderFromSourceFile( QOpenGLShader::Fragment, ":/fshader.fsh" );
    if ( !m_program->link() )
    {
        qWarning( "Error: unable to link a shader program" );
        return;
    }
    m_posAttr = m_program->attributeLocation( "posAttr" );
    m_colAttr = m_program->uniformLocation( "colAttr" );
    m_matrixUniform = m_program->uniformLocation( "matrix" );
    changeProection();
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void Widget::drawTest() {
    QList<QVector3D> curvePoints;
    m_transformObjects.pop_back();
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));
    QImage texture = QImage(":/steel.jpg");

    QVector3D point1 = QVector3D(x_pos, y_pos, z_pos);
//  your tesiting object
    SimpleObject3D* obj = FigureBuilder::initDiskSector(texture, point1, 5, 2 * M_PI, 0.01, false);

    obj->rotate(QQuaternion::fromAxisAndAngle(x_s, y_s, z_s, a_s));
    obj->translate(QVector3D(x_pos, y_pos, z_pos));

    m_groups.last()->addObject(obj);
    m_transformObjects.append(m_groups.last());
}

void Widget::initHyperboloid(QVector3D center, double R, double delta) {
    QList<QVector3D> curvePoints;
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));

    countSteps = 10;

    QVector3D center1 = QVector3D(0.0, 0.0, -15);
    double r = 15;
    QVector3D center2 = center1;
    QImage texture = QImage(":/steel.jpg");
    center2.setZ(center1.z() - 0.01);
    int height = r;

    //передняя часть основания
    int horizontalCount = 10;
    for (int i = 0; i <= height; i++) {
        for (double j = 0; j < M_PI; j += M_PI / horizontalCount) {
            float currentRadius = (i) ;
            QVector3D point1 = QVector3D(center1.x() + currentRadius * cos(j) , center1.y() + currentRadius * sin(j), center1.z() + i);
            QVector3D point2 = QVector3D(center1.x() + currentRadius * cos(j + M_PI / horizontalCount), center1.y() + currentRadius * sin(j + M_PI / horizontalCount), center1.z() + i);


            m_groups.last()->addObject(FigureBuilder::initBelt(texture, point1, point2, 0.05, 0.05, 0.01));
        }
        std::cout << "drawed " << i << std::endl;
    }
    std::cout << "start draw vertical sticks" << std::endl;
    for (double i = 0; i <= M_PI; i += M_PI / countSteps) {
        //рисуем вертикальные палки
        m_groups.last()->addObject(FigureBuilder::initBelt(texture, center1, QVector3D(center1.x() + r * cos(i), center1.y() + r * sin(i), center1.z() + 15), 0.05, 0.05, 0.01));
    }

    // сиденье
    texture = QImage(":/3.jpg");
    QVector3D point1 = QVector3D(center1.x(), center1.y(), center1.z() + height);
    QVector3D point2 = QVector3D(center1.x(), center1.y(), center1.z() + height + 1);
    m_groups.last()->addObject(FigureBuilder::initBelt(texture, point1, point2, 14, 14, 0.01));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(texture, point1, 14, 2 * M_PI, 0.01));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(texture, point2, 14, 2 * M_PI, 0.01));

//    // металлический ободок
//    point1 = QVector3D(center1.x(), center1.y(), center1.z() + height);
//    m_groups.last()->addObject(FigureBuilder::initDiskSector(texture, point1, 15, 2 * M_PI, 0.01));

    //газлифт
    texture = QImage(":/steel.jpg");
    point1 = QVector3D(center1.x(), center1.y(), center1.z() - 1);
    point2 = QVector3D(center1.x(), center1.y(), center1.z() + 5);

    m_groups.last()->addObject(FigureBuilder::initBelt(texture, point1, point2, 0.5, 0.5, 0.01));



    //ножки
    point1 = QVector3D(center1.x(), center1.y(), center1.z() - 1);
    m_groups.last()->addObject(FigureBuilder::initCube(texture, 15, 1, 1, point1));
    m_groups.last()->addObject(FigureBuilder::initCube(texture, 1, 1, 15, point1));

    // каркас спинки
    int maxHeight = 40.0;
    r = 15;
    int j = 0;// для шагов
    int stepDelta = 5;

    for (double i = 0; i < M_PI; i += M_PI / countSteps) {
        //рисуем вертикальные палки
        if (j >= countSteps / 2) {
            stepDelta = -5;
        }

        j++;
        m_groups.last()->addObject(FigureBuilder::initBelt(texture, center1, QVector3D(center1.x() - r * cos(i), center1.y() - r * sin(i), center1.z() + r), 0.05, 0.05, 0.05));
        std::cout << r << std::endl;
        r += stepDelta;

    }

    // задняя часть основания
    stepDelta = 1;
    r = 0;
    for (int i = 0; i <= height; i++) {
        int horizontalCounter = 0;
        for (double k = 0; k < M_PI; k += M_PI / horizontalCount) {
            if (horizontalCounter >= countSteps /2) {
                point1 = QVector3D(center1.x() - (r + 5)* cos(k) , center1.y() - (r + 5) * sin(k), center1.z() + i + 5);
                point2 = QVector3D(center1.x() - (r) * cos(k + M_PI / horizontalCount), center1.y() - (r) * sin(k + M_PI / horizontalCount), center1.z() + (i));
            } else {
                point1 = QVector3D(center1.x() - r * cos(k) , center1.y() - r * sin(k), center1.z() + i);
                point2 = QVector3D(center1.x() - (r + 5) * cos(k + M_PI / horizontalCount), center1.y() - (r + 5) * sin(k + M_PI / horizontalCount), center1.z() + (i + 5));
            }

            m_groups.last()->addObject(FigureBuilder::initBelt(texture, point1, point2, 0.05, 0.05, 0.01));
            horizontalCounter++;
        }
        r += stepDelta;
    }
    // сетка спинки
    r--;
    std::cout << r << std::endl;
    int d = 5;
    for (double k = 0; k < M_PI/2 ; k += M_PI / horizontalCount) {
        for (int i = 15; i <= r; i++) {
            point1 = QVector3D(center1.x() - (i) * cos(k) , center1.y() - (i) * sin(k), center1.z() + i);
            point2 = QVector3D(center1.x() - (i + d) * cos(k + M_PI / horizontalCount), center1.y() - (i + d) * sin(k + M_PI / horizontalCount), center1.z() + (i + d));
            std::cout << point1.x() << " " << point1.y() << " " << point1.z() << std::endl;
            m_groups.last()->addObject(FigureBuilder::initBelt(texture, point1, point2, 0.05, 0.05, 0.01));

            point1 = QVector3D(center1.x() - (i)* cos(M_PI - k) , center1.y() - (i) * sin(M_PI - k), center1.z() + i);
            point2 = QVector3D(center1.x() - (i + d) * cos(M_PI - (k + M_PI / horizontalCount)), center1.y() - (i +d) * sin(M_PI - (k + M_PI / horizontalCount)), center1.z() + (i + d));
            m_groups.last()->addObject(FigureBuilder::initBelt(texture, point1, point2, 0.05, 0.05, 0.01));
        }
        r += d;

    }

    double r1 = 0;

    // спинка
    delta = 0.1;
//    double lowerBound = 10;
//    double upperBound = 11;

//    point1 = QVector3D(10, 0, 20);
////    point2 = QVector3D(16, 22, 15);
//    SimpleObject3D* obj = FigureBuilder::initDiskSector(texture, point1, 5, 2 * M_PI, 0.01, false);
//    obj->rotate(QQuaternion::fromAxisAndAngle(4.0, 0.0, 1.0, 90 + 5));
//    m_groups.last()->addObject(obj);


//    for(double x = lowerBound - delta; x <= upperBound + delta; x += delta)
//    {
//        curvePoints.push_back(QVector3D(FigureBuilder::calculteLemniscatePoint(x, c), 0, x - 25));
//    }

////    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));

//    for(int i = 0; i < curvePoints.size() - 1; i++)
//    {
//        double r = curvePoints[i + 1].x() - curvePoints[i].x();
//        double r1 = curvePoints[i].x();
//        double r2 = curvePoints[i + 1].x();

//        QVector3D center1 = QVector3D(0.0, 0.0, curvePoints[i].z());
//        QVector3D center2 = QVector3D(0.0, 0.0, curvePoints[i + 1].z());
//        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/paper2.jpg"), center2, center1, r2, r1));
//    }

//    for(int i = 0; i < countSteps; i++)
//    {
////        double r = curvePoints[i + 1].x() - curvePoints[i].x();
//        double r2 = (i  + 1)*0.1;

//        QVector3D center1 = QVector3D(0.0, 0.0, i * 0.1 + 10);
//        QVector3D center2 = QVector3D(0.0, 0.0, (i  + 1)*0.1 + 10);
//        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/paper2.jpg"), center2, center1, r2, r1));
//        r1 = r2;
//    }

    std::cout << "done" << std::endl;

    m_groups.last()->addObject(FigureBuilder::initLine(QImage(":/123.jpg"), QVector3D(1, 0.0, 0), QVector3D(0.0, 3, 5), r));


    m_transformObjects.append(m_groups.last());
}

void Widget::initScene() {
    QList<QVector3D> curvePoints;
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));

    QImage p = QImage(":/7.jpg");

//    m_groups.last()->addObject(FigureBuilder::initCube(p, 25));
//    m_groups.last()->addObject(FigureBuilder::initCylinder(QVector3D(0, -10, -25), QVector3D(0, -10, 0), 10, 0.1, 0.01));

//    m_groups.last()->addObject(FigureBuilder::initCylinder(QVector3D(10, 0, -25), QVector3D(10, 0, 5), 5, 5, 0.01));

//    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), QVector3D(10, 0, -25), 5));
//    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), QVector3D(10, 0, -10), 5));

//    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), QVector3D(0, 0, 0), QVector3D(0, 0,10), 0.1, 5, 0.1));


    m_transformObjects.append(m_groups.last());
}

void Widget::initWoodenWheel()
{
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));

    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), QVector3D(0.0, 0.0, 0.0), 1, 2 * M_PI));

    QVector3D center1 = QVector3D(0.0, 0.0, 0.0);
    QVector3D center2 = QVector3D(0.0, 0.0, -0.25);
    double r1 = 1;
    double r2 = 1.5;
    double step = 0.1;
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), center1, center2, r1, r2, step));

    center1 = QVector3D(0.0, 0.0, -0.25);
    center2 = QVector3D(0.0, 0.0, -1.0);
    r1 = 1.5;
    r2 = 1.5;
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), center1, center2, r1, r2, step));

    center1 = QVector3D(0.0, 0.0, -1.0);
    center2 = QVector3D(0.0, 0.0, -1.25);
    r1 = 1.5;
    r2 = 1;
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), center1, center2, r1, r2, step));

    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), center2, r2, 2 * M_PI, 0.1, true));

    m_transformObjects.append(m_groups.last());
}

void Widget::initSandGlass()
{
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));
    double radiusDelta = 0.05;

    double rTop = 0.25;
    double rBorder1 = 0.5;
    double rMiddle = 0.05;
    double rBorder2 = 0.5;
    double rBottom = 0.25;

    QVector3D middle = QVector3D(0.0, -25.0, 0.0);

    double radiusCurrent = rMiddle;
    QVector3D currentCenter = middle;
    QVector3D nextCenter = QVector3D(currentCenter.x(),
                                     currentCenter.y(),
                                     currentCenter.z() + 0.5 * radiusCurrent);

    for(double r = rMiddle + radiusDelta; r < rBorder1; r += radiusDelta)
    {
        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), nextCenter, currentCenter, r, radiusCurrent));

        radiusCurrent = r;
        currentCenter = nextCenter;

        nextCenter = QVector3D(currentCenter.x(),
                               currentCenter.y(),
                               currentCenter.z() + 0.5 * r);
    }


    //currentCenter = nextCenter;
    nextCenter = QVector3D(currentCenter.x(),
                           currentCenter.y(),
                           currentCenter.z() + 0.25);
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"),
                                                       nextCenter,
                                                       currentCenter,
                                                       rBorder1,
                                                       rBorder1));

    radiusDelta = 0.01;
    for(double r = radiusCurrent + radiusDelta; r > rTop; r -= radiusDelta)
    {
        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), nextCenter, currentCenter, r, radiusCurrent));

        radiusCurrent = r;
        currentCenter = nextCenter;

        nextCenter = QVector3D(currentCenter.x(),
                               currentCenter.y(),
                               currentCenter.z() + 0.1 * r * r);
    }
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), currentCenter, radiusCurrent, 2 * M_PI));


    radiusDelta = 0.05;
    radiusCurrent = rMiddle;
    currentCenter = middle;
    nextCenter = QVector3D(currentCenter.x(),
                           currentCenter.y(),
                           currentCenter.z() - 0.5 * radiusCurrent);

    for(double r = rMiddle + radiusDelta; r < rBorder2; r += radiusDelta)
    {
        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), currentCenter, nextCenter, radiusCurrent, r));

        radiusCurrent = r;
        currentCenter = nextCenter;

        nextCenter = QVector3D(currentCenter.x(),
                               currentCenter.y(),
                               currentCenter.z() - 0.5 * r);
    }

    nextCenter = QVector3D(currentCenter.x(),
                           currentCenter.y(),
                           currentCenter.z() - 0.25);
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"),
                                                       currentCenter,
                                                       nextCenter,
                                                       rBorder1,
                                                       rBorder1));

    radiusDelta = 0.01;
    for(double r = radiusCurrent + radiusDelta; r > rBottom; r -= radiusDelta)
    {
        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), currentCenter, nextCenter, radiusCurrent, r));

        radiusCurrent = r;
        currentCenter = nextCenter;

        nextCenter = QVector3D(currentCenter.x(),
                               currentCenter.y(),
                               currentCenter.z() - 0.1 * r * r);
    }
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), currentCenter, radiusCurrent, 2 * M_PI, 0.1, true));

    m_transformObjects.append(m_groups.last());
}

void Widget::initSandGlass2(double lowerBound, double upperBound, double c, double delta)
{
    // рисуем силуэт часов
    QList<QVector3D> curvePoints;

    for(double x = lowerBound - delta; x <= upperBound + delta; x += delta)
    {
        curvePoints.push_back(QVector3D(FigureBuilder::calculteLemniscatePoint(x, c), 0, x - 25));
    }

    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));

    for(int i = 0; i < curvePoints.size() - 1; i++)
    {
        double r = curvePoints[i + 1].x() - curvePoints[i].x();
        double r1 = curvePoints[i].x();
        double r2 = curvePoints[i + 1].x();

        QVector3D center1 = QVector3D(0.0, 0.0, curvePoints[i].z());
        QVector3D center2 = QVector3D(0.0, 0.0, curvePoints[i + 1].z());
        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/paper2.jpg"), center2, center1, r2, r1));
    }

    // рисуем верх и низ часов
    double r = curvePoints.first().x();
    QVector3D center1 = QVector3D(0.0, 0.0, curvePoints.first().z());
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/paper2.jpg"), center1, r, 2 * M_PI, 0.1, true));
    r = curvePoints.last().x();
    center1 = QVector3D(0.0, 0.0, curvePoints.last().z());
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/paper2.jpg"), center1, r, 2 * M_PI, 0.1, false));

    // рисуем каркас вокруг часов
    r = curvePoints.first().x() * 2;
    center1 = QVector3D(0.0, 0.0, curvePoints.first().z());
    QVector3D center2 = center1;
    center2.setZ(center1.z() - 0.25);
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), center1, center2, r, r, 0.1));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), center1, r, 2 * M_PI, 0.1, false));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), center2, r, 2 * M_PI, 0.1, true));

    r = curvePoints.last().x() * 2;
    center1 = QVector3D(0.0, 0.0, curvePoints.last().z());
    center2 = center1;
    center2.setZ(center1.z() + 0.25);
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), center2, center1, r, r, 0.1));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), center1, r, 2 * M_PI, 0.1, true));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":/123.jpg"), center2, r, 2 * M_PI, 0.1, false));

    double offset = 0.1;
    r = 0.05;
    center1 = QVector3D(curvePoints.last().x() + offset, curvePoints.last().x() + offset, curvePoints.last().z());
    center2 = QVector3D(curvePoints.first().x() + offset, curvePoints.first().x() + offset, curvePoints.first().z());
    SimpleObject3D* obj = FigureBuilder::initBelt(QImage(":/123.jpg"), center2, center1, r, r, 0.1);
    m_groups.last()->addObject(obj);

    center1 = QVector3D(curvePoints.last().x() + offset, curvePoints.last().x() + offset, curvePoints.last().z());
    center2 = QVector3D(curvePoints.first().x() + offset, curvePoints.first().x() + offset, curvePoints.first().z());
    obj = FigureBuilder::initBelt(QImage(":/123.jpg"), center2, center1, r, r, 0.1);
    obj->rotate(QQuaternion::fromAxisAndAngle(0.0, 0.0, 1.0, 120));
    m_groups.last()->addObject(obj);

    center1 = QVector3D(curvePoints.last().x() + offset, curvePoints.last().x() + offset, curvePoints.last().z());
    center2 = QVector3D(curvePoints.first().x() + offset, curvePoints.first().x() + offset, curvePoints.first().z());
    obj = FigureBuilder::initBelt(QImage(":/123.jpg"), center2, center1, r, r, 0.1);
    obj->rotate(QQuaternion::fromAxisAndAngle(x_s, y_s, z_s, a_s));
    m_groups.last()->addObject(obj);

    m_transformObjects.append(m_groups.last());
}

void Widget::initBook(QVector3D centerTop, QVector3D centerBottom, double sideX, double sideY, double delta)
{
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D));

    double offset = std::fabs(std::fabs(centerTop.z()) - std::fabs(centerBottom.z()));

    QVector3D centerMiddle = QVector3D(centerTop.x() - centerBottom.x(), centerTop.y() - centerBottom.y(), centerBottom.z() + offset / 2);
    QVector3D center = QVector3D(0.0, 0.0, centerBottom.z());

    for(double z = centerBottom.z() + delta; z <= centerMiddle.z(); z += delta)
    {
        center = QVector3D(center.x() + delta / 2.5, center.y(), z);
        m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":paper1.jpg"),
                                                                     center,
                                                                     sideX - delta * 10, sideY - delta * 10, delta));
        // часть корешка книги
        QVector3D rootCenter = QVector3D((center.x() + (sideX - delta * 10) / 2 + delta / 2),
                                         (center.y()),
                                         (center.z()));
        m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"),
                                                                     rootCenter,
                                                                     delta * 5, sideY, delta));

    }

    center.setZ(center.z() + delta);

    for(double z = center.z() + delta; z < centerTop.z(); z += delta)
    {
        m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":paper1.jpg"),
                                                                     center,
                                                                     sideX - delta * 10, sideY - delta * 10, delta));

        // часть корешка книги
        QVector3D rootCenter = QVector3D((center.x() + (sideX - delta * 10) / 2 + delta / 2),
                                         (center.y()),
                                         (center.z()));
        m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"),
                                                                     rootCenter,
                                                                     delta * 5, sideY, delta));
        center = QVector3D(center.x() - delta / 2.5, center.y(), z);
    }

    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), centerTop, sideX, sideY, 0.1));
    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), centerBottom, sideX, sideY, 0.1));
    m_transformObjects.append(m_groups.last());
}

void Widget::initSimpleBook(const QImage &texture, QVector3D centerTop, QVector3D centerBottom, double sideX, double sideY, double delta)
{
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D));
    double offset = std::fabs(std::fabs(centerTop.z()) - std::fabs(centerBottom.z())) - delta;

    m_groups.last()->addObject(FigureBuilder::initParallelepiped(texture, centerTop, sideX, sideY, delta));

    //qDebug() << offset << centerTop.z() - delta;

    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":paper1.jpg"),
                                                                 QVector3D(centerTop.x(),
                                                                           centerTop.y(),
                                                                           centerTop.z() - delta),
                                                                 sideX - delta * 2,
                                                                 sideY - delta * 2,
                                                                 offset));
    m_groups.last()->addObject(FigureBuilder::initParallelepiped(texture,
                                                                 QVector3D(centerTop.x() + sideX / 2 - delta / 2,
                                                                           centerTop.y(),
                                                                           centerTop.z() - delta),
                                                                 delta,
                                                                 sideY,
                                                                 offset));
    m_groups.last()->addObject(FigureBuilder::initParallelepiped(texture, centerBottom, sideX, sideY, delta));
    m_transformObjects.append(m_groups.last());
}

void Widget::initStairs(QVector3D centerTop, QVector3D centerBottom, double width, double delta)
{
    double side = 0.25;
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D));

    QVector3D topLeftCenter = QVector3D(centerTop.x() - width / 2,
                                        centerTop.y(),
                                        centerTop.z());
    QVector3D topRightCenter = QVector3D(centerTop.x() + width / 2,
                                         centerTop.y(),
                                         centerTop.z());

    QVector3D bottomLeftCenter = QVector3D(centerBottom.x() - width / 2,
                                           centerBottom.y(),
                                           centerBottom.z());
    QVector3D bottomRightCenter = QVector3D(centerBottom.x() + width / 2,
                                            centerBottom.y(),
                                            centerBottom.z());

    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), topLeftCenter, side, side, 0.05));
    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), topRightCenter, side, side, 0.05));

    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), bottomLeftCenter, side, side, 0.05));
    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), bottomRightCenter, side, side, 0.05));


    m_groups.last()->addObject(FigureBuilder::initSquareBelt(QImage(":123.jpg"),
                                                             topLeftCenter, side, side,
                                                             bottomLeftCenter, side, side));

    m_groups.last()->addObject(FigureBuilder::initSquareBelt(QImage(":123.jpg"),
                                                             topRightCenter, side, side,
                                                             bottomRightCenter, side, side));

    //double height = std::fabs(centerTop.z()) - std::fabs(centerBottom.z());

    double centerTopX = centerTop.y();
    double centerBottomX = centerBottom.y();

    //qDebug() << centerBottom.y() << centerTop.y();

    double centerTopZ = centerTop.z();
    double centerBottomZ = centerBottom.z();

    double katetHorizontal = centerBottomX - centerTopX;
    double katetVertical = centerTopZ - centerBottomZ;

    double tgA = katetVertical / katetHorizontal;

    double cosA = sqrt(1 / (tgA * tgA + 1));
    double sinA = sqrt(1 - cosA * cosA);
    //qDebug() << cosA << tgA << katetVertical << katetHorizontal;


    /*SimpleObject3D* obj = FigureBuilder::initBelt(QImage(":123.jpg"),
                                                  QVector3D(centerBottom.x(),
                                                            centerBottom.y(),
                                                            centerBottom.z() + width / 2),
                                                  QVector3D(centerBottom.x(),
                                                            centerBottom.y(),
                                                            centerBottom.z() - width / 2),
                                                  sqrt(side) / 2,
                                                  sqrt(side) / 2,
                                                  0.1);
    obj->translate(QVector3D(10 * width, -katetHorizontal, 0.0));
    obj->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, 90));
    m_groups.last()->addObject(obj);*/

    for(double level = centerBottom.z(); level <= centerTop.z() - delta; level += delta)
    {
        //qDebug() << level + width / 2 << level - width / 2;

        QVector3D center1 = QVector3D(centerBottom.x() / sinA,
                                     centerBottom.y() / sinA,
                                     centerBottom.z() / sinA - level);
        QVector3D center2 = QVector3D(centerBottom.x() / sinA,
                                     centerBottom.y() / sinA,
                                     centerBottom.z() / sinA + width);

        SimpleObject3D* obj = FigureBuilder::initParallelepiped(QImage(":123.jpg"),
                                                                center1,
                                                                width,
                                                                side,
                                                                side);

        /*SimpleObject3D* obj = FigureBuilder::initBelt(QImage(":123.jpg"),
                                                      center1,
                                                      center2,
                                                      sqrt(side) / 2,
                                                      sqrt(side) / 2,
                                                      0.1);*/
        //obj->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, 90));
        //obj->translate(QVector3D((centerBottom.z() - level) * width, -katetHorizontal, (centerBottom.z() - level)));
        obj->rotate(QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, -(90 - acos(cosA) * 180 / M_PI)));

        //obj->translate(QVector3D());
        //obj->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, 90));
        //obj->translate(QVector3D((centerBottom.z() - level) * width, 1.5 * width, (centerBottom.z() - level)));

        m_groups.last()->addObject(obj);
    }

    m_transformObjects.append(m_groups.last());
}

void Widget::initStairs(QVector3D centerTop, double width, double height, double side, double delta, double angle)
{
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D));

    QVector3D topLeftCenter = QVector3D(centerTop.x() - width / 2,
                                        centerTop.y(),
                                        centerTop.z());
    QVector3D topRightCenter = QVector3D(centerTop.x() + width / 2,
                                         centerTop.y(),
                                         centerTop.z());

    QVector3D bottomLeftCenter = QVector3D(centerTop.x() - width / 2,
                                           centerTop.y(),
                                           centerTop.z() - height);
    QVector3D bottomRightCenter = QVector3D(centerTop.x() + width / 2,
                                            centerTop.y(),
                                            centerTop.z() - height);

    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), topLeftCenter, side, side, 0.05));
    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), topRightCenter, side, side, 0.05));

    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), bottomLeftCenter, side, side, 0.05));
    m_groups.last()->addObject(FigureBuilder::initParallelepiped(QImage(":123.jpg"), bottomRightCenter, side, side, 0.05));


    m_groups.last()->addObject(FigureBuilder::initSquareBelt(QImage(":123.jpg"),
                                                             topLeftCenter, side, side,
                                                             bottomLeftCenter, side, side));

    m_groups.last()->addObject(FigureBuilder::initSquareBelt(QImage(":123.jpg"),
                                                             topRightCenter, side, side,
                                                             bottomRightCenter, side, side));

    QVector3D centerBottom = QVector3D(centerTop.x(),
                                       centerTop.y(),
                                       centerTop.z() - height);

    for(double level = centerBottom.z() + delta; level < centerTop.z(); level += delta)
    {
        /*SimpleObject3D* obj = FigureBuilder::initParallelepiped(QImage(":123.jpg"),
                                                                QVector3D(centerBottom.x(),
                                                                          centerBottom.y(),
                                                                          level),
                                                                width,
                                                                side,
                                                                side);*/
        SimpleObject3D* obj = FigureBuilder::initStair(QImage(":123.jpg"),
                                                       QVector3D(centerBottom.x() - width / 2,
                                                                 centerBottom.y(),
                                                                 level),
                                                       QVector3D(centerBottom.x() + width / 2,
                                                                 centerBottom.y(),
                                                                 level),
                                                       side / 2,
                                                       side / 2,
                                                       0.1);

        m_groups.last()->addObject(obj);
    }

    m_groups.last()->rotate(QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, angle));
    m_transformObjects.append(m_groups.last());
}

void Widget::initBarrel(double leftOffset, double delta)
{
    QList<QVector3D> curvePoints;

    for(double x = leftOffset / 2; x < 0.0; x += delta)
    {
        curvePoints.push_back(QVector3D(x + fabs(leftOffset * 2), 0.0, 2 * sqrt(-x)));
    }

    QList<QVector3D> invertedPoints;
    for(auto point : curvePoints)
    {
        invertedPoints.push_front(QVector3D(point.x(), point.y(), -point.z()));
    }

    curvePoints += invertedPoints;

    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));
                                                        // QVector3D(0, 1, 1)));
    for(int i = 0; i < curvePoints.size() - 1; i++)
    {
        double r1 = curvePoints[i].x();
        double r2 = curvePoints[i + 1].x();

        QVector3D center1 = QVector3D(0.0, 0.0, curvePoints[i].z());
        QVector3D center2 = QVector3D(0.0, 0.0, curvePoints[i + 1].z());
        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":2.jpg"),
                                                           center1,
                                                           center2,
                                                           r1, r2));
    }

    double r = curvePoints.first().x() - 0.1;
    QVector3D center = QVector3D(0.0, 0.0, curvePoints.first().z() - 0.1);
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":123.jpg"), center, r, 2 * M_PI, 0.1, false));
    double round1 = r + 0.3;
    double round2 = r + 0.31;
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":1.jpg"), QVector3D(0.0, 0.0, curvePoints.first().z() - 0.6), round2, 2 * M_PI, 0.1, false));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":1.jpg"), QVector3D(0.0, 0.0, curvePoints.first().z() - 0.5), round1, 2 * M_PI, 0.1, false));
    m_groups.last()->addObject(FigureBuilder::initCylinder(QVector3D(0.0, 0.0, curvePoints.first().z() - 0.6), QVector3D(0.0, 0.0, curvePoints.first().z() - 0.5), round2, round1, 0.1));


    double r1 = curvePoints.first().x();
    double r2 = curvePoints.first().x() - 0.1;
    center = QVector3D(0.0, 0.0, curvePoints.first().z());
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":2.jpg"), center, center, r2, r1));

    r = r2;
    QVector3D center1 = QVector3D(0.0, 0.0, curvePoints.first().z() - 0.1);
    QVector3D center2 = QVector3D(0.0, 0.0, curvePoints.first().z());
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":2.jpg"), center1, center2, -r, -r));

    r = curvePoints.last().x() - 0.1;
    center = QVector3D(0.0, 0.0, curvePoints.last().z() + 0.1);
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":123.jpg"), center, r, 2 * M_PI, 0.1, true));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":1.jpg"), QVector3D(0.0, 0.0, curvePoints.last().z() + 0.6), round2, 2 * M_PI, 0.1, false));
    m_groups.last()->addObject(FigureBuilder::initDiskSector(QImage(":1.jpg"), QVector3D(0.0, 0.0, curvePoints.last().z() + 0.5), round1, 2 * M_PI, 0.1, false));
    m_groups.last()->addObject(FigureBuilder::initCylinder(QVector3D(0.0, 0.0, curvePoints.last().z() + 0.6), QVector3D(0.0, 0.0, curvePoints.last().z() + 0.5), round2, round1, 0.1));

    r1 = curvePoints.last().x();
    r2 = curvePoints.last().x() - 0.1;
    center = QVector3D(0.0, 0.0, curvePoints.last().z());
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":2.jpg"), center, center, r1, r2));

    r = r2;
    center1 = QVector3D(0.0, 0.0, curvePoints.last().z() + 0.1);
    center2 = QVector3D(0.0, 0.0, curvePoints.last().z());
    m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":2.jpg"), center1, center2, -r, -r));

    m_transformObjects.append(m_groups.last());
}

void Widget::getX(int j) {
    m_projectionMatrix.rotate(j - pred[0], 1.0f, 0.0f, 0.0f);
    pred[0] = j;
this->update();}

void Widget::getY(int j) {
    m_projectionMatrix.rotate(j - pred[1], 0.0f, 1.0f, 0.0f);
    pred[1] = j;
this->update();}

void Widget::getZ(int j) {
    m_projectionMatrix.rotate(j - pred[2], 0.0f, 0.0f, 1.0f);
    pred[2] = j;
this->update();}

void Widget::getSize(int s) {
    this->size = s;
    this->countSteps = s+1;
    std::cout << s << std::endl;

     this->update();
}

void Widget::getR(int r) {
    this->R = r;
this->update();
}

void Widget::changeProection() {
    proection = !proection;
    m_projectionMatrix.setToIdentity();
    if (proection) {
        m_projectionMatrix.ortho(-12.0f,12.0f,-12.0f,12.0f,0.1f,1000.0f);
    } else {
        m_projectionMatrix.perspective(45.0f, 5, 0.01f, 1000.0f);
    }
    this->update();
}


void Widget::setLightX(int val) {
    this->xLight = val;
    this->update();
}

void Widget::setLightY(int val) {
    this->yLight = val;
    this->update();
}

void Widget::setLightZ(int val) {
    this->zLight = val;
    this->update();
}

void Widget::setLightAmbient(int val) {
    this->ambient= val;
    this->update();
}

void Widget::setX(int val) {
    x_s = val;
    std::cout << "x_s:" << x_s  << " y_s:" << y_s << " z_s:" << z_s << " a_s:" << a_s<< std::endl;
    this->update();
}

void Widget::setY(int val) {
    y_s = val;
    std::cout << "x_s:" << x_s  << " y_s:" << y_s << " z_s:" << z_s << " a_s:" << a_s<< std::endl;
    this->update();

}

void Widget::setZ(int val) {
    z_s = val;
    std::cout << "x_s:" << x_s  << " y_s:" << y_s << " z_s:" << z_s << " a_s:" << a_s<< std::endl;
    this->update();

}

void Widget::setA(int val) {
    a_s = val;
    std::cout << "x_s:" << x_s  << " y_s:" << y_s << " z_s:" << z_s << " a_s:" << a_s<< std::endl;
    this->update();

}

void Widget::setPosX(int val) {
    x_pos = val;
    std::cout << "x_v:" << x_pos  << " y_v:" << y_pos << " z_v:" << z_s << std::endl;
    this->update();
}

void Widget::setPosY(int val) {
    y_pos = val;
    std::cout << "x_v:" << x_pos  << " y_v:" << y_pos << " z_v:" << z_s << std::endl;
    this->update();

}

void Widget::setPosZ(int val) {
    z_pos = val;
    std::cout << "x_v:" << x_pos  << " y_v:" << y_pos << " z_v:" << z_s << std::endl;
    this->update();

}
