#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent):QGLWidget(parent)
{
    setGeometry(50,50,500,500);
}
void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    initShaders();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if ( !m_program->bind() )
        return;
    glLoadIdentity();
    gluLookAt(10.0f,10.0f,10.0f,
              0.0f,0.0f,0.0f,
              0.0f,10.0f,1.0f);
    draw();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport( 0, 0, width, height );
}

void GLWidget::draw(){

    m_program->setUniformValue( m_matrixUniform, matrix );
    m_program->setUniformValue(m_colAttr, 1.0f,1.0f,1.0f,1.0f);

    //оси
    GLfloat vect[]={
        -10.0f,0.0f,-10.0f,
        10.0f,0.0f,-10.0f,
        0.0f,-10.0f,-10.0f,
        0.0f,10.0f,-10.0f,
        0.0f,0.0f,-10.0f,
        0.0f,0.0f,10.0f

    };
    glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vect );
    glEnableVertexAttribArray( m_posAttr );
    glDrawArrays( GL_LINES, 0, 6);
    glDisableVertexAttribArray(  m_posAttr );
    m_program->setUniformValue(m_colAttr, 0.5f,0.7f,1.0f,1.0f);

    //    //прямоугольники
    //    GLfloat vect1[]={
    //        -size,size/1.5f,-size/1.3f,
    //         size,size/1.5f,-size/1.3f,
    //         size,-size/1.5f,-size/1.3f,
    //         -size,-size/1.5f,-size/1.3f,
    //         -size,size/1.5f,-size/1.3f,

    //        -size,size/1.5f,-size/4.0f,
    //         size,size/1.5f,-size/4.0f,
    //         size,-size/1.5f,-size/4.0f,
    //         -size,-size/1.5f,-size/4.0f,
    //         -size,size/1.5f,-size/4.0f,

    //        -size*1.5f,size*1.5f,size/2.7f,
    //         size*1.5f,size*1.5f,size/2.7f,
    //         size*1.5f,-size*1.5f,size/2.7f,
    //         -size*1.5f,-size*1.5f,size/2.7f,
    //         -size*1.5f,size*1.5f,size/2.7f,

    //        -size*1.5f,size*1.5f,size*1.4f,
    //         size*1.5f,size*1.5f,size*1.4f,
    //         size*1.5f,-size*1.5f,size*1.4f,
    //         -size*1.5f,-size*1.5f,size*1.4f,
    //         -size*1.5f,size*1.5f,size*1.4f,

    //    };
    //    glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vect1 );
    //    glEnableVertexAttribArray( m_posAttr );
    //    glDrawArrays( GL_LINE_STRIP, 0, 20);
    //    glDisableVertexAttribArray(  m_posAttr );

    //    //соединение прямоугольников
    //    GLfloat vect2[]={
    //        size,size/1.5f,-size/1.3f,
    //        size,size/1.5f,-size/4.0f,
    //        size,-size/1.5f,-size/1.3f,
    //        size,-size/1.5f,-size/4.0f,
    //        -size,-size/1.5f,-size/1.3f,
    //        -size,-size/1.5f,-size/4.0f,

    //        size,size/1.5f,-size/4.0f,
    //        size*1.5f,size*1.5f,size/2.7f,
    //        size,-size/1.5f,-size/4.0f,
    //        size*1.5f,-size*1.5f,size/2.7f,
    //        -size,-size/1.5f,-size/4.0f,
    //        -size*1.5f,-size*1.5f,size/2.7f,

    //        size*1.5f,size*1.5f,size/2.7f,
    //        size*1.5f,size*1.5f,size*1.4f,
    //        size*1.5f,-size*1.5f,size/2.7f,
    //        size*1.5f,-size*1.5f,size*1.4f,
    //        -size*1.5f,-size*1.5f,size/2.7f,
    //        -size*1.5f,-size*1.5f,size*1.4f,

    //        -size*1.3f,size*1.3f,size*1.4f,
    //        size*1.3f,size*1.3f,size*1.4f,
    //        size*1.3f,size*1.3f,size*1.4f,
    //        size*1.3f,-size*1.3f,size*1.4f,
    //        size*1.3f,-size*1.3f,size*1.4f,
    //        -size*1.3f,-size*1.3f,size*1.4f,
    //        -size*1.3f,-size*1.3f,size*1.4f,
    //        -size*1.3f,size*1.3f,size*1.4f,

    //    };
    //    glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vect2 );
    //    glEnableVertexAttribArray( m_posAttr );
    //    glDrawArrays( GL_LINES, 0, 26);
    //    glDisableVertexAttribArray(  m_posAttr );

    //круги
    int lenght = 21;
    GLfloat vec[lenght];

    for(double t=0;t<2*M_PI;t+=0.01){
        //        int ctr = -1; ОТЛАДИТЬ
        //        float rctr = 2.0f;
        //        float yctr = 10.25f;
        //        while (ctr < 14)
        //        {
        //            ctr++;
        //            qDebug() << QString::number(ctr);
        //            vec[ctr] = R/rctr*qCos(t) + 0.1*qCos(t);
        //            vec[ctr++] = -size*1.5f + yctr;
        //            vec[ctr++] = size/1.15f - R/rctr*qSin(t);
        //            rctr = rctr + 0.3f;
        //            yctr = yctr + 0.5f;
        //        }

        // Работает
        for(int i = 0; i < lenght - 3; i+=3) {
            vec[i] = R/(2.0 + i*0.25)*qCos(t)+0.1*qCos(t);
            vec[i+1]=-size*1.5f+(10.25 + i*0.5f);
            vec[i+2]=size/1.15f - R/(2.0 + i*0.25)*qSin(t);
        }
//        vec[0]=R/2.0f*qCos(t)+0.1*qCos(t);
//        vec[1]=-size*1.5f+10.25f;
//        vec[2]=size/1.15f - R/2.0f*qSin(t);

//        vec[3]=R/2.25f*qCos(t)+0.1*qCos(t);
//        vec[4]=-size*1.5f+10.75f;
//        vec[5]=size/1.15f - R/2.25f*qSin(t);

//        vec[6]=R/2.5f*qCos(t)+0.1*qCos(t);
//        vec[7]=-size*1.5f+11.25f;
//        vec[8]=size/1.15f - R/2.5f*qSin(t);

//        vec[9]=R/2.75f*qCos(t)+0.1*qCos(t);
//        vec[10]=-size*1.5f+11.75f;
//        vec[11]=size/1.15f - R/2.75f*qSin(t);


//        vec[12]=R/3.0f*qCos(t)+0.1*qCos(t);
//        vec[13]=-size*1.5f+12.25f;
//        vec[14]=size/1.15f - R/3.0f*qSin(t);

//        vec[15]=R/3.25f*qCos(t)+0.1*qCos(t);
//        vec[16]=-size*1.5f+12.75f;
//        vec[17]=size/1.15f - R/3.25f*qSin(t);

//        vec[18]=R/3.75f*qCos(t)+0.1*qCos(t);
//        vec[19]=-size*1.5f+13.25f;
//        vec[20]=size/1.15f - R/3.75f*qSin(t);

        glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0,vec);
        glEnableVertexAttribArray( m_posAttr );
        glDrawArrays( GL_POINTS, 0, 5);
        glDisableVertexAttribArray(  m_posAttr );

    }



    //    for(double t=0;t<2*M_PI;t+=0.01){ // was
    //        vec[0]=R/3.0f*qCos(t);
    //        vec[1]=-size*1.5f;
    //        vec[2]=size/1.15f - R/3.0f*qSin(t);

    //        vec[3]=R/3.0f*qCos(t);
    //        vec[4]=-size*1.5f-1.0f;
    //        vec[5]=size/1.15f - R/3.0f*qSin(t);

    //        vec[6]=R*qCos(t);
    //        vec[7]=-size*1.5f-2.0f;
    //        vec[8]=size/1.15f - R*qSin(t);

    //        vec[9]=R*qCos(t);
    //        vec[10]=-size*1.5f-2.2f;
    //        vec[11]=size/1.15f - R*qSin(t);


    //        vec[12]=R/3.0f*qCos(t)+0.1*qCos(t);
    //        vec[13]=-size*1.5f+12.5f;
    //        vec[14]=size/1.15f - R/3.0f*qSin(t);

    //        glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0,vec);
    //        glEnableVertexAttribArray( m_posAttr );
    //        glDrawArrays( GL_POINTS, 0, 5);
    //        glDisableVertexAttribArray(  m_posAttr );
    //    }

    //линии
    //drawVLines(-size*1.5f, -size*1.5f-1.0f, R/3.0f);
    //drawVLines(-size*1.5f-2.0f, -size*1.5f-2.2f, R);
    GLfloat vec1[3];
    float i=10.25f;
    m_program->setUniformValue(m_colAttr, 0.5f,0.3f,0.2f,1.0f);

    for(double x = 2.0f; x < 3.1f; x += 0.01f) // частота по
    {
        for(double t=0; t<2*M_PI; t+=0.1f)  // частота по кругу
        {
            vec1[0] = R/x*qCos(t);
            vec1[1] = -size*1.5f + i;
            vec1[2] = size/1.15f - R/x*qSin(t);

            //            vec1[0]=x*qCos(t);
            //            vec1[1]=-size*1.5f-1.0f - i;
            //            vec1[2]=size/1.15f - x*qSin(t);

            glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vec1);
            glEnableVertexAttribArray( m_posAttr );
            glDrawArrays( GL_POINTS, 0, 1);
            glDisableVertexAttribArray(  m_posAttr );
        }
        if(i < 12.26/*-size*1.5f-1.0f - i >= -size*1.5f-2.0f*/) { // частота по высоте
            i=i+0.05f;
        } else {
            i = 10.25f;
        }
    }
    m_program->release();
}

void GLWidget::drawVLines(double y0, double y1, float r) {
    GLfloat vec1[3];
    for(double y=y0;y>y1;y-=0.03) {
        for(double t=0; t<2*M_PI; t+=0.35) {
            vec1[0]=r*qCos(t);
            vec1[1]=y;
            vec1[2]=size/1.15f - r*qSin(t);
            glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0,vec1);
            glEnableVertexAttribArray( m_posAttr );
            glDrawArrays( GL_POINTS, 0, 1);
            glDisableVertexAttribArray(  m_posAttr );
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
            !extensionsString.contains("GL_ARB_shading_language_100"))
    {
        exit(0);
    }

    //compile shaders
    m_program = new QOpenGLShaderProgram( this );
    m_program->addShaderFromSourceFile( QOpenGLShader::Vertex, ":/vshader.glsl" );
    m_program->addShaderFromSourceFile( QOpenGLShader::Fragment, ":/fshader.glsl" );
    if ( !m_program->link() )
    {
        qWarning( "Error: unable to link a shader program" );
        return;
    }
    m_posAttr = m_program->attributeLocation( "posAttr" );
    m_colAttr = m_program->uniformLocation( "colAttr" );
    m_matrixUniform = m_program->uniformLocation( "matrix" );

    matrix.ortho(-12.0f,12.0f,-12.0f,12.0f,-12.0f,12.0f);

    glEnable(GL_PROGRAM_POINT_SIZE);
}

void GLWidget::getX(int j){
    matrix.rotate(j-pred[0],1.0f,0.0f,0.0f);
    pred[0]=j;
    this->updateGL();
}

void GLWidget::getY(int j){
    matrix.rotate(j-pred[1],0.0f,1.0f,0.0f);
    pred[1]=j;
    this->updateGL();
}

void GLWidget::getZ(int j){
    matrix.rotate(j-pred[2],0.0f,0.0f,1.0f);
    pred[2]=j;
    this->updateGL();
}

void GLWidget::getSize(int s){
    this->size=s;
    this->updateGL();
}

void GLWidget::getR(int r){
    this->R=r;
    this->updateGL();
}
