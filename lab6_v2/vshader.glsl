attribute highp vec4 posAttr;
uniform highp mat4 matrix;
uniform lowp vec4 colAttr;
varying lowp vec4 col;

void main()
{

    col = colAttr;
    gl_Position = matrix * posAttr;
}
