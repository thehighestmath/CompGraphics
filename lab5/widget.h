#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QApplication>
#include <QMouseEvent>
#include <QTimer>

struct VertexData {
    VertexData() {}
    VertexData(QVector3D p, QVector2D t, QVector3D n) : position(p), texCoord(t), normal(n) {}
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void slotMove();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    QVector<VertexData> paintTwoTriangles(float x, float y, float z, float nextZ);
    void initWave();
    void initRectangle();
private:
    const float dx = 0.01;
    const float dy = 0.01;
    float m_z;

    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_shaderProgramm;

    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    QVector2D m_mousePosition;
    QQuaternion m_rotation;

    QTimer m_timer;
    float deltaTime = 0;
};

#endif // WIDGET_H
