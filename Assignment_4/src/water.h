#pragma once

#include "mygl/base.h"

struct WaveParams
{
    float amplitude;
    float phi;
    float omega;
    Vector2D direction;
};

struct WaterSim
{
    WaveParams parameter[3] =
    {
        { 0.6f,  0.5f,  0.25f, normalize(Vector2D{1.0f,  1.0f}) },
        { 0.7f,  0.25f, 0.1f,  normalize(Vector2D{1.0f, -1.0f}) },
        { 0.1f,  0.9f,  0.9f,  normalize(Vector2D{-1.0f, 0.0f}) },
    };

    float accumTime = 0.0f;
};

float waterHeight(const WaterSim& sim, Vector2D position);
Matrix4D waterBuoyancyRotation(const WaterSim& sim, const Vector2D& v0, const Vector2D& v1, const Vector2D& v2);
