#include <GL/glu.h>
#include <GL/gl.h>

void doSomething() {
    GLUnurbsObj *nurbs;
    nurbs = gluNewNurbsRenderer() ;
    float nurbsctrl[][3]= {
        {-60,-60,0},
        {-60,-20,0},
        {-60,20,0},
        {-60,60,0},
        {-20,-60,0},
        {-20,-20,-80},
        {-20,20,-80},
        {-20,60,0},
        {20,-60,0},
        {20,-20,-80},
        {20,20,-80},
        {20,60,0},
        {60,-60, 0},
        {60,-20, 0},
        {60, 20, 0},
        {60, 60, 0}};

    float outer[]={0,0,1,0,1,1,0,1,0,0} ;
    float knots[8]={0,0,0,0,1,1,1,1};
    gluBeginSurface(nurbs);
    gluNurbsSurface(nurbs,8,knots,8,knots,12,3, nurbsctrl[0],4,4,GL_MAP2_VERTEX_3);
    gluBeginTrim(nurbs);
    gluPwlCurve(nurbs,5,outer,2,GLU_MAP1_TRIM_2); gluEndTrim(nurbs);
    gluBeginTrim(nurbs);
    gluPwlCurve(nurbs,5,outer,2,GLU_MAP1_TRIM_2); gluEndTrim(nurbs);
    gluEndSurface(nurbs);
}
