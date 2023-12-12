#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

struct wave_params
{
    float amplitude;
    float phi;
    float omega;
    vec2 direction;
};

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform float time;
uniform wave_params water_sim[3];

out vec3 tNormal;
out vec3 tFragPos;

float wave_height(vec2 pos, float t, wave_params param)
{
    return param.amplitude * sin(dot(normalize(param.direction), pos) * param.omega + t * param.phi);
}

vec2 wave_delta(vec2 pos, float t, wave_params param)
{
    float dx = param.amplitude * param.direction.x * param.omega * cos(dot(normalize(param.direction), pos) * param.omega + t * param.phi);
    float dy = param.amplitude * param.direction.y * param.omega * cos(dot(normalize(param.direction), pos) * param.omega + t * param.phi);

    return vec2(dx, dy);
}

void main(void)
{
    vec3 position = aPosition;
    vec3 normal = aNormal;
    vec2 delta = vec2(0, 0);

    for(int i = 0; i < 3; i++)
    {
        position.y += wave_height(position.xz, time, water_sim[i]);
        delta += wave_delta(position.xz, time, water_sim[i]);
    }

    normal = normalize(cross(vec3(0, delta.y, 1), vec3(1, delta.x, 0)));

    gl_Position = uProj * uView * uModel * vec4(position, 1.0);
    tFragPos = vec3(uModel * vec4(position, 1.0));
    tNormal = mat3(transpose(inverse(uModel))) * normal;
}
