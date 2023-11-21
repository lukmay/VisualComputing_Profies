#version 330 core

in vec4 tColor;
in vec3 tFragPos;
out vec4 FragColor;

void main(void)
{
    FragColor = tColor;
}
