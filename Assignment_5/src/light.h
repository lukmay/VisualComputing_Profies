#pragma once

#include <mygl/base.h>

struct Light_Directional
{
    Vector3D direction;

    Vector3D ambient;
    Vector3D color;
};

struct Light_Spot
{
    Vector3D position;
    Vector3D direction;

    Vector3D color;

    float constant;
    float linear;
    float quadratic;

    float cutoff;

    bool enabled = true;
};
