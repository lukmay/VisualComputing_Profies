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
};

struct Surface
{
    vec3 normal;
    vec3 fragPos;
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

vec3 blinnPhongIllumination(in Surface surface, in Light light, in Material material, in Camera camera){
    /* ambient */
    vec3 ambient = light.ambientCoeff * material.ambient * light.ambientColor;
    /* diffuse */
    vec3 diffuse = light.diffuseCoeff * material.diffuse * light.color * clamp(dot(surface.normal, normalize(light.direction)), 0.0f, 1.0f);
    /* specular */
    vec3 halfway = normalize(normalize(surface.fragPos - camera.position) - normalize(light.direction));
    vec3 specular = light.specularCoeff * material.specular * light.color * pow(dot(surface.normal, halfway), material.shininess);

    return ambient + diffuse + specular;
}

void main(void)
{

    Surface surface = Surface(normalize(tNormal), tFragPos);

    vec3 illumination = blinnPhongIllumination(surface, uLight, uMaterial, uCamera);
    FragColor = vec4(illumination, 1.0);
}
