varying vec2 texCoord; // значение текстурных координат
// именно этого фрагмента
void main(void)
{
// закрасим в режиме RGBA
gl_FragColor=vec4(texCoord.x,1,1,1.0);
}