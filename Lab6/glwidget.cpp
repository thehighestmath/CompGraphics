#include "glwidget.h"
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
template<typename T>
void printArr(T arr, int size){
    for (int i = 0; i < size; i++) {
        if (i % 3 == 0) {
            std::cout << std::endl;
        }
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setGeometry(50, 50, 500, 500);
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    initShaders();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (!m_program->bind())
        return;
    glLoadIdentity();
    gluLookAt(10.0f, 10.0f, 10.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 10.0f, 1.0f);
    draw();
}

void GLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

void GLWidget::draw() {

    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setUniformValue(m_colAttr, 1.0f, 1.0f, 1.0f, 1.0f);

    //оси
    GLfloat vect[] = {
            -10.0f, 0.0f, -10.0f,
            10.0f, 0.0f, -10.0f,
            0.0f, -10.0f, -10.0f,
            0.0f, 10.0f, -10.0f,
            0.0f, 0.0f, -10.0f,
            0.0f, 0.0f, 10.0f
    };
    // xyz оси
//    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vect);
//    glEnableVertexAttribArray(m_posAttr);
//    glDrawArrays(GL_LINES, 0, 6);
//    glDisableVertexAttribArray(m_posAttr);
//    m_program->setUniformValue(m_colAttr, 0.5f, 0.7f, 1.0f, 1.0f);


    // гипердолоид
    /*
    x = x0 + a*ch(u) * cos(v)
    y = y0 + b*sh(u) * sin(v)
    z = z0 + c*sh(u)
    v [0; 2pi) == t
    u (-inf; +inf)
    */

    //круги
    int countCircles = 7;
    int countSteps = 47;
    float u;
    GLfloat vec[countCircles * 3 ];
    GLfloat totalPoints[(countSteps * 3 * countCircles + countSteps * 3 * (countCircles - 2))];
    int j = 0;
    for (double t = 0; t < 2 * M_PI; t += 2.0 * M_PI / countSteps / 100) {
        m_program->setUniformValue(m_colAttr, t / 2 / M_PI, t / 2 / M_PI, 1.0f, 1.0f);
        // Работает
        for (int i = 0; i < countCircles; i += 1) {
            if (i != 0){
                if (i > countCircles){
                    u = -log(i - countCircles);
                } else {
                    u = log(i);
                }
            } else {
                u = 0;
            }
            // old way
//            vec[3 * i] = R / (2.0 + 3 *  i * 0.25) * qCos(t) + 0.1 * qCos(t);
//            vec[3 * i + 1] = -size * 1.5f + (10.25 + 3 * i * 0.5f);
//            vec[3 * i + 2] = size / 1.15f - R / (2.0 + 3 * i * 0.25) * qSin(t);

            // trying new way
            vec[3 * i] = x0 + R * cosh(u) * cos(t);
            vec[3 * i + 1] = y0 + R * sinh(u) * sin(t);
            vec[3 * i + 2] = z0 + R * sinh(u);
        }

        glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vec);
        glEnableVertexAttribArray(m_posAttr);
        glDrawArrays(GL_POINTS, 0, countCircles);
        glDisableVertexAttribArray(m_posAttr);
    }

    //линии
    j = 0;
    for (double t = 0; t < 2 * M_PI; t += 2.0 * M_PI / countSteps) {
        m_program->setUniformValue(m_colAttr, t / 2 / M_PI, t / 2 / M_PI, 1.0f, 1.0f);
        // Работает
        for (int i = 0; i < countCircles; i += 1) {
            if (i != 0){
                if (i > countCircles){
                    u = -log(i - countCircles);
                } else {
                    u = log(i);
                }
            } else {
                u = 0;
            }
            // old way
//            vec[3 * i] = R / (2.0 + 3 *  i * 0.25) * qCos(t) + 0.1 * qCos(t);
//            vec[3 * i + 1] = -size * 1.5f + (10.25 + 3 * i * 0.5f);
//            vec[3 * i + 2] = size / 1.15f - R / (2.0 + 3 * i * 0.25) * qSin(t);

            // trying new way
            vec[3 * i] = x0 + R * cosh(u) * cos(t);
            vec[3 * i + 1] = y0 + R * sinh(u) * sin(t);
            vec[3 * i + 2] = z0 + R * sinh(u);

            totalPoints[j] = vec[3 * i];
            totalPoints[j + 1] = vec[3 * i + 1];
            totalPoints[j + 2] = vec[3 * i + 2];
            j += 3;
            if (i != 0 && i != countCircles - 1){
                totalPoints[j] = vec[3 * i];
                totalPoints[j + 1] = vec[3 * i + 1];
                totalPoints[j + 2] = vec[3 * i + 2];
                j += 3;
            }
        }
    }

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, totalPoints);
    glEnableVertexAttribArray(m_posAttr);
    glDrawArrays(GL_LINES, 0, countSteps * countCircles + countSteps * (countCircles - 2));
    glDisableVertexAttribArray(m_posAttr);
    m_program->setUniformValue(m_colAttr, 0.5f, 0.7f, 1.0f, 1.0f);

    m_program->release();
}

void GLWidget::drawVLines(double y0, double y1, float r) {
    GLfloat vec1[3];
    for (double y = y0; y > y1; y -= 0.03) {
        for (double t = 0; t < 2 * M_PI; t += 0.35) {
            vec1[0] = r * qCos(t);
            vec1[1] = y;
            vec1[2] = size / 1.15f - r * qSin(t);
            glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vec1);
            glEnableVertexAttribArray(m_posAttr);
            glDrawArrays(GL_POINTS, 0, 1);
            glDisableVertexAttribArray(m_posAttr);
        }
    }
}

void GLWidget::initShaders() {
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
        !extensionsString.contains("GL_ARB_shading_language_100")) {
        exit(0);
    }

    //compile shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl");
    if (!m_program->link()) {
        qWarning("Error: unable to link a shader program");
        return;
    }
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->uniformLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    matrix.ortho(-12.0f, 12.0f, -12.0f, 12.0f, -12.0f, 12.0f);

    glEnable(GL_PROGRAM_POINT_SIZE);
}

void GLWidget::getX(int j) {
    matrix.rotate(j - pred[0], 1.0f, 0.0f, 0.0f);
    pred[0] = j;
    this->updateGL();
}

void GLWidget::getY(int j) {
    matrix.rotate(j - pred[1], 0.0f, 1.0f, 0.0f);
    pred[1] = j;
    this->updateGL();
}

void GLWidget::getZ(int j) {
    matrix.rotate(j - pred[2], 0.0f, 0.0f, 1.0f);
    pred[2] = j;
    this->updateGL();
}

void GLWidget::getSize(int s) {
    this->size = s;
    this->updateGL();
}

void GLWidget::getR(int r) {
    this->R = r;
    this->updateGL();
}
