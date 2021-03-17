uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
//uniform int       iFrame;                // shader playback frame
//uniform float     iChannelTime[4];       // channel playback time (in seconds)
//uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
//uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
//uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
//uniform vec4      iDate;                 // (year, month, day, time in seconds)
//uniform float     iSampleRate;           // sound sample rate (i.e., 44100)

varying vec2 texCoord; // значение текстурных координат
// именно этого фрагмента
void main(void)
{
    vec2 xy = texCoord.xy; //Получаем координаты текущего пикселя
    xy.x = xy.x / iResolution.x; //Делим координаты на размер экрана
    xy.y = xy.y / iResolution.y;
    // Теперь x для самого левого пикселя равен 0, а для самого правого равен 1
    vec4 solidRed = vec4(1,0.0,0.0,1.0); //Теперь он стал чёрным
    solidRed.r = xy.x; //Присваиваем красному компоненту нормализованное значение x
    gl_FragColor = solidRed;
//    gl_FragColor = vec4(texCoord.x,0,0,1.0);
}