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

    initHyperboloid(QVector3D(0,0,0), R, 0.1);

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
    m_program->setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_program->setUniformValue("u_lightPower", 1.0f);

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

    // вектор, вокруг которого выполняется поворот, перпендикулярен направлению движения мыши
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

    // TODO
//    m_projectionMatrix.ortho(-12.0f,12.0f,-12.0f,12.0f,-12.0f,50.0f);
        changeProection();
        //    gluPerspective();
//    glOrtho()

    glEnable(GL_PROGRAM_POINT_SIZE);
}

void Widget::initHyperboloid(QVector3D center, double R, double delta) {
    QList<QVector3D> curvePoints;
    m_groups.push_back(QSharedPointer<Group3D>(new Group3D()));


    int countCircles = 7;
    countSteps = 20;

    QVector3D center1 = QVector3D(0.0, 0.0, 0.0);
    double r = R;
    QVector3D center2 = center1;
    center2.setZ(center1.z() - 0.01);
    int height = countCircles;
    float beulifulConst = 8;
    QList<QVector3D> normal, back;
    // рисование кругов
    for (int i = 0; i < countCircles; i++) {
        center1 = QVector3D(0.0, 0.0, i - beulifulConst);
        center2 = QVector3D(0.0, 0.0, i+0.05f - beulifulConst);
        double r1 = r/(i+1);
        double r2 = r/(i+2);
        //
        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), center1, center2, r/(i+1), r/(i+1), 0.01));

        for (double t = 0; t < 2 * M_PI; t += 2.0 * M_PI / countSteps) {
            m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"),
                                                               QVector3D(center1.x() + r1 * cos(t), center1.y() + r1 * sin(t), center1.z()),
                                                               QVector3D(center2.x() + r2 * cos(t), center2.y() + r2 * sin(t), center2.z() + 1),
                                                               0.01, 0.01, 0.01));
        }

        center1 = QVector3D(0.0, 0.0, 2 * height - i - 0.2f - beulifulConst);
        center2 =  QVector3D(0.0, 0.0, 2 * height - i + 0.05f - 0.2f - beulifulConst);
        m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), center1, center2, r/(i+1), r/(i+1), 0.01));
        for (double t = 0; t < 2 * M_PI; t += 2.0 * M_PI / countSteps) {
            m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"),
                                                               QVector3D(center1.x() + r1 * cos(t), center1.y() + r1 * sin(t), center1.z()),
                                                               QVector3D(center2.x() + r2 * cos(t), center2.y() + r2 * sin(t), center2.z() - 1),
                                                               0.01, 0.01, 0.01));
        }
        std::cout << i << std::endl;

        if (i == countCircles - 1) {
            center1 = QVector3D(0.0, 0.0, 2 * height - i - 0.2f - beulifulConst - 1);
            center2 =  QVector3D(0.0, 0.0, 2 * height - i + 0.05f - 0.2f - beulifulConst - 1);
            m_groups.last()->addObject(FigureBuilder::initBelt(QImage(":/123.jpg"), center1, center2, r/(i+1), r/(i+1), 0.01));

        }
    }

    m_groups.last()->addObject(FigureBuilder::initLine(QImage(":/123.jpg"), QVector3D(1, 0.0, 0), QVector3D(0.0, 3, 5), r));


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
        m_projectionMatrix.ortho(-12.0f,12.0f,-12.0f,12.0f,0.1f,100.0f);
    } else {
        m_projectionMatrix.perspective(45.0f, 5, 0.01f, 100.0f);
    }
    this->update();
}
