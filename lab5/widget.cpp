#include "widget.h"

Widget::Widget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    m_z = -4;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotMove()));

    m_timer.start(50);
}


void Widget::slotMove()
{
    deltaTime += 0.01;

    update();
}


Widget::~Widget() {}

void Widget::initializeGL()
{
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initShaders();

}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / qreal(h ? h : 1);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.1f, 100.0f);

}

void Widget::initWave()
{
    QVector<VertexData> vertexes;

    const float lengthX = 3.0;
    const float lengthY = 2.0;

    int pairTrianglesCount = 4;

    for (float x = 0.0; x <= lengthX - dx; x += dx) {
        for (float y = 0.0; y <= lengthY - dy; y += dy) {

            vertexes.append(VertexData(QVector3D(x, y + dy, 0), QVector2D(x / lengthX, (y + dy) / lengthY), QVector3D(0.0, 0.0, 0.1)));
            vertexes.append(VertexData(QVector3D(x, y, 0), QVector2D(x / lengthX, y / lengthY), QVector3D(0.0, 0.0, 0.1)));
            vertexes.append(VertexData(QVector3D(x + dx, y + dy, 0), QVector2D((x + dx) / lengthX, (y + dy) / lengthY), QVector3D(0.0, 0.0, 0.1)));
            vertexes.append(VertexData(QVector3D(x + dx, y, 0), QVector2D((x + dx) / lengthX, y / lengthY), QVector3D(0.0, 0.0, 0.1)));

            pairTrianglesCount += 4;
        }
    }

    QVector<GLuint> indexes;

    for (int i = 0; i < pairTrianglesCount; i += 4) {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(-1.0f, -1.0f, m_z);
    modelViewMatrix.rotate(m_rotation);

    m_shaderProgramm.bind();
    m_shaderProgramm.setUniformValue("MVPMatrix", m_projectionMatrix * modelViewMatrix);
    m_shaderProgramm.setUniformValue("modelMatrix", modelViewMatrix);
    m_shaderProgramm.setUniformValue("time", deltaTime);

    initWave();

    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    int offset = 0;

    int location = m_shaderProgramm.attributeLocation("a_position");
    m_shaderProgramm.enableAttributeArray(location);
    m_shaderProgramm.setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    location = m_shaderProgramm.attributeLocation("a_textcoord0");
    m_shaderProgramm.enableAttributeArray(location);
    m_shaderProgramm.setAttributeBuffer(location, GL_FLOAT, offset, 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
    m_shaderProgramm.release();

}

void Widget::initShaders()
{
    if (!m_shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Vertex, "://vshader.vsh"))
        close();

    if (!m_shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Fragment, "://fshader.fsh"))
        close();

    if (!m_shaderProgramm.link())
        close();

    if (!m_shaderProgramm.bind())
        close();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        m_mousePosition = QVector2D(event->localPos());
    }

    event->accept();
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton) return;

    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angle = diff.length() / 2.0f;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);

    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        m_z += 0.25f;
    } else if (event->delta() < 0) {
        m_z -= 0.25f;
    }

    update();
}


