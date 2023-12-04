#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 color;
    vec3 position;
};

in vec3 tNormal;
in vec3 tFragPos;
in vec3 tViewDir;

out vec4 FragColor;

uniform Material uMaterial;
uniform Light uLight[4];
uniform vec3 uViewPos;

void main(void)
{
    int ambientCoeff = 1;
    vec3 ambient = ambientCoeff * uMaterial.diffuse * uLight[0].color;

    int diffuseCoeff = 1;
    vec3 diffuse = diffuseCoeff * uMaterial.diffuse * uLight[1].color * max(0.0f, dot(normalize(tNormal), normalize(uLight[0].position - tFragPos)));

    int specularCoeff = 1;
    vec3 viewDir = normalize(tViewDir);
    vec3 halfway = normalize(dot(normalize(tNormal), normalize(uLight[1].position)) + viewDir);
    vec3 specular = specularCoeff * uMaterial.diffuse * uLight[2].color * pow(max(0.0f, dot(normalize(tNormal), halfway)), uMaterial.shininess);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
