#pragma once

#include "mygl/model.h"

#include "water.h"

#include <vector>

struct Boat
{
    enum eControl
    {
        RUDDER_LEFT,
        RUDDER_RIGHT,
        THROTTLE_UP,
        THROTTLE_DOWN,
        CONTROL_COUNT
    };

    std::vector<Model> partModel;

    Matrix4D transformation = Matrix4D::identity();
    Vector3D position = {0.0, 0.0, 0.0};
    Vector3D angles = {0.0, 0.0, 0.0};
};

Boat boatLoad(const std::string& filepath);
void boatDelete(Boat& boat);
void boatMove(Boat& boat, const WaterSim& waterSim, bool control[], float dt);
