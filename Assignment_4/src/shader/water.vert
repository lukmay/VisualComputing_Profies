#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform float u_lastTime;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform vec2 u_direction_0;
uniform vec2 u_direction_1;
uniform vec2 u_direction_2;

out vec3 tNormal;
out vec3 tFragPos;
out vec2 tUV;

float wave(float distance1, float distance2, float distance3)
{
    return sin(0.25 * distance1 + 0.5 * u_lastTime) * 0.6 +
    sin(0.1 * distance2 + 0.25 * u_lastTime) * 0.7 +
    sin(0.9 * distance3 + 0.9 * u_lastTime) * 0.1;
}
float distance1(vec4 vertex)
{
    return dot(vertex.xz, u_direction_0);
}
float distance2(vec4 vertex)
{
    return dot(vertex.xz, u_direction_1);
}
float distance3(vec4 vertex)
{
    return dot(vertex.xz, u_direction_2);
}

float dxwave(float distance1, float distance2, float distance3)
{
    return cos(0.25 * distance1 + 0.5 * u_lastTime) * 0.6 * 0.25 * u_direction_0.x +
    cos(0.1 * distance2 + 0.25 * u_lastTime) * 0.7 * 0.1 * u_direction_1.x +
    cos(0.9 * distance3 + 0.9 * u_lastTime) * 0.1 * 0.9 * u_direction_2.x;
}
float dzwave(float distance1, float distance2, float distance3)
{
    return cos(0.25 * distance1 * 0.5 + u_lastTime) * 0.6 * 0.25 * u_direction_0.y +
    cos(0.1 * distance2 * 0.25 + u_lastTime) * 0.7 * 0.1 * u_direction_1.y +
    cos(0.9 * distance3 * 0.9 + u_lastTime) * 0.1 * 0.9 * u_direction_2.y;
}
void main(void)
{
    vec4 vertex = vec4(aPosition, 1.0);

    vertex.y = wave(distance1(vertex), distance2(vertex), distance3(vertex));
    gl_Position = uProj * uView * uModel * vertex;
    tFragPos = vec3(uModel * vertex);
    tNormal = normalize(cross(-(vec3(1, dxwave(distance1(vertex), distance2(vertex), distance3(vertex)), 0)),
                                vec3(0, dzwave(distance1(vertex), distance2(vertex), distance3(vertex)), 1))); //mat3(transpose(inverse(uModel))) * aNormal;
    tUV = aUV;
}
