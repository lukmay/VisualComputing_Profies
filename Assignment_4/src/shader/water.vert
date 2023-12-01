#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
uniform float u_lastTime;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 tNormal;
out vec3 tFragPos;
out vec2 tUV;

const float pi = 3.14285714286;
void main(void)
{
    vec4 vertex = vec4(aPosition, 1.0);
    vec2 origin1 = vertex.xz + vec2(300.0, 300.0);
    vec2 origin2 = vertex.xz + vec2(-300.0, 300.0);
    vec2 origin3 = vertex.xz + vec2(300.0, -300.0);
    vec2 origin4 = vertex.xz + vec2(-300.0, -300.0);

    float distance1 = length(origin1);
    float distance2 = length(origin2);
    float distance3 = length(origin3);
    float distance4 = length(origin4);

    float wave = sin(3.3 * pi * distance1 * 0.13 + u_lastTime) * 0.125 +
    sin(3.2 * pi * distance2 * 0.12 + u_lastTime) * 0.125 +
    sin(3.1 * pi * distance3 * 0.24 + u_lastTime) * 0.125 +
    sin(3.5 * pi * distance4 * 0.32 + u_lastTime) * 0.125;

    vertex.y += wave;

    gl_Position = uProj * uView * uModel * vertex;
    tFragPos = vec3(uModel * vec4(aPosition, 1.0));
    tNormal = mat3(transpose(inverse(uModel))) * aNormal;
    tUV = aUV;
}
