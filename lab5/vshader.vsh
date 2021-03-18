attribute vec4 a_position;
attribute vec2 a_textcoord0;

uniform mat4 MVPMatrix;
uniform mat4 modelMatrix;
uniform float time;
varying vec3 normal;
varying vec4 fragPos;

// f'(time) = 2 * cos((newPos.x - time) * 10.0) / 5.0;
// tg(a) = f'(x0)
// a = arctg(f'(x0))
// n = {cos(a), 0, sin(a)}


void main(void)
{
    vec4 newPos = vec4(a_position);
    newPos.z = sin((newPos.x - time) * 10.0) / 5.0;

    float a = atan(2.0 * cos((newPos.x - time) * 10.0) / 5.0);
    vec3 n = vec3(cos(a), 0, sin(a));

    gl_Position = MVPMatrix * newPos;
    normal = n;
    fragPos = newPos * modelMatrix;
}
