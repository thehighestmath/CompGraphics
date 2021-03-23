varying vec3 normal;
varying vec4 fragPos;
vec3 lightPos = vec3(2.0, 2.0, 2.0);
vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main(void)
{
    vec3 lightDir = normalize(lightPos - fragPos.xyz);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (diffuse + 0.5) * vec3(1.0, 0.3, 0.5);
    gl_FragColor = vec4(result, 1.0f);
}

