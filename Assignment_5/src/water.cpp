#include "water.h"

float waveHeight(Vector2D pos, float t, const WaveParams& params)
{
    return params.amplitude * sin(dot(normalize(params.direction), pos) * params.omega + t * params.phi);
}

float waterHeight(const WaterSim &sim, Vector2D position)
{
    return waveHeight(position, sim.accumTime, sim.parameter[0]) + waveHeight(position, sim.accumTime, sim.parameter[1]) + waveHeight(position, sim.accumTime, sim.parameter[2]);
}

Matrix4D waterBuoyancyRotation(const WaterSim &sim, const Vector2D &v0, const Vector2D &v1, const Vector2D &v2)
{
    Vector3D d0 = { v0.x, waterHeight(sim, v0), v0.y };
    Vector3D d1 = { v1.x, waterHeight(sim, v1), v1.y };
    Vector3D d2 = { v2.x, waterHeight(sim, v2), v2.y };

    auto n0 = normalize(d1 - d0);
    auto n1 = normalize(d2 - d0);
    auto n2 = normalize(d1 - d2);

    auto up = normalize(cross(n1, n0));
    auto right = n2;
    auto front = normalize(cross(right, up));

    return Matrix4D(Matrix3D(right.x, up.x, -front.x,
                             right.y, up.y, -front.y,
                             right.z, up.z, -front.z));
}
