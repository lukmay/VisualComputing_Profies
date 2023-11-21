#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec4 tColor;
out vec3 tFragPos;

void main(void)
{
    gl_Position = uProj * uView * uModel * vec4(aPosition, 1.0);
    tColor = aColor;
    tFragPos = vec3(uModel * vec4(aPosition, 1.0));
}
