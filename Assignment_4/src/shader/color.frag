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
uniform Light uFrontLight2;
uniform Light uRedLight;

void main(void)
{
    float theta1 = dot(normalize(uFrontLight1.position - tFragPos), normalize(-uFrontLight1.direction));
    float theta2 = dot(normalize(uFrontLight2.position - tFragPos), normalize(-uFrontLight2.direction));
    float theta3 = dot(normalize(uRedLight.position - tFragPos), normalize(-uRedLight.direction));

    vec3 ambient = uLight.ambientCoeff * uMaterial.ambient * uMaterial.diffuse * uLight.ambientColor;
    vec3 diffuse = uLight.diffuseCoeff * uMaterial.diffuse * uLight.color * clamp(dot((normalize(tNormal)), normalize(uLight.direction)),0.0f ,1.0f);

    vec3 halfway = normalize(normalize(tFragPos + uCamera.position) + normalize(uLight.direction));
    vec3 specular = uLight.specularCoeff * uMaterial.specular * uLight.color * pow(dot(normalize(tNormal), halfway), uMaterial.shininess);

    vec3 illumination = ambient + diffuse + specular;

    if(theta1 > uFrontLight1.cutOff){
        illumination += uFrontLight1.color * 0.1f;
    }

    if(theta2 > uFrontLight2.cutOff){
        illumination += uFrontLight2.color * 0.1f;
    }
    if(theta3 > uRedLight.cutOff){
        illumination += uRedLight.color * 0.5f;
    }

    FragColor = vec4(illumination, 1.0);
}
