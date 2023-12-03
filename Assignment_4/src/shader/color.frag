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
uniform Light uLight;
uniform vec3 uViewPos;

void main(void)
{
    /* with the globalAmbientColor we can control the day/night setting.
    (0.8f,0.8f,0.8f) could be daylight
    (0.4f,0.4f,0.4f) could be night
    */
    int ambientCoeff = 1;
    vec3 globalAmbientLightColor = vec3(1.0f,1.0f,1.0f);
    vec3 ambient = ambientCoeff * uMaterial.diffuse * globalAmbientLightColor;

    int diffuseCoeff = 1;
    vec3 lightColor = vec3(1.0f,1.0f,1.0f);
    vec3 lightDir = normalize(uLight.position - tFragPos);
    vec3 diffuse = diffuseCoeff * uMaterial.diffuse * lightColor * max(0.0f, dot(normalize(tNormal), lightDir));

    int specularCoeff = 1;
    vec3 viewDir = normalize(tViewDir);
    vec3 lightColor2 = vec3(1.0f,0.0f,0.0f);
    vec3 halfway = normalize(lightDir + viewDir);
    vec3 specular = specularCoeff * uMaterial.diffuse * lightColor2 * pow(max(0.0f,dot(normalize(tNormal), halfway)), uMaterial.shininess);


    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
