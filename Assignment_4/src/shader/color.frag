#version 330 core

struct Material
{
    vec3 diffuse;
};

out vec4 FragColor;

uniform Material uMaterial;

void main(void)
{
    FragColor = vec4(uMaterial.diffuse, 1.0);
}
