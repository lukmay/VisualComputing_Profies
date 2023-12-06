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
    float ambientCoeff;
    float diffuseCoeff;
    float specularCoeff;
    vec3 ambientColor;
    vec3 color;
    vec3 direction;
    float cutOff;
    vec3 position;
};

struct Camera
{
    vec3 position;
};

in vec3 tNormal;
in vec3 tFragPos;
in vec3 tViewDir;

out vec4 FragColor;

uniform Material uMaterial;
uniform Light uLight;
uniform Camera uCamera;
uniform Light uFrontLight1;


void main(void)
{
    float theta = dot(normalize(uFrontLight1.position - tFragPos), normalize(-uFrontLight1.direction));

    vec3 ambient = uLight.ambientCoeff * uMaterial.ambient * uMaterial.diffuse * uLight.ambientColor;
    vec3 diffuse = uLight.diffuseCoeff * uMaterial.diffuse * uLight.color * clamp(dot((normalize(tNormal)), normalize(uLight.direction)),0.0f ,1.0f);

    vec3 halfway = normalize(normalize(tFragPos + uCamera.position) + normalize(uLight.direction));
    vec3 specular = uLight.specularCoeff * uMaterial.specular * uLight.color * pow(dot(normalize(tNormal), halfway), uMaterial.shininess);

    vec3 illumination = theta > uFrontLight1.cutOff ? (ambient + diffuse + specular) + (uFrontLight1.color * 0.1 ): (ambient + diffuse + specular);
    FragColor = vec4(illumination, 1.0);
}
