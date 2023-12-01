#include "boat.h"

Boat boatLoad(const std::string& filepath)
{
    Boat boat;
    boat.partModel = modelLoad(filepath);
    return boat;
}

void boatDelete(Boat& boat)
{
    for(auto& model : boat.partModel)
    {
        modelDelete(model);
    }

    boat.partModel.clear();
}

void boatMove(Boat& boat, const WaterSim& waterSim, bool control[], float dt)
{
    /* retrieve input for controls */
    float throttle = + control[Boat::eControl::THROTTLE_UP] - control[Boat::eControl::THROTTLE_DOWN];
    float rudder = + control[Boat::eControl::RUDDER_LEFT] - control[Boat::eControl::RUDDER_RIGHT];

    /* rotate due to rudde control */
    boat.angles.y += throttle * rudder * dt;
    auto rotation = Matrix4D::rotationY(boat.angles.y);

    /* move boat along direction vector */
    boat.position += rotation * (2.0f * dt * throttle * Vector4D(0.0, 0.0, 1.0, 0.0));

    /* find triangle to compute wave orientation */
    auto center = Vector2D(boat.position.x, boat.position.z);
    auto r0 = rotation * Vector4D( 0.0, 0.0,  1.8, 0.0);
    auto r1 = rotation * Vector4D(-0.8, 0.0, -1.9, 0.0);
    auto r2 = rotation * Vector4D( 0.8, 0.0, -1.9, 0.0);

    auto v0 = center + Vector2D{r0.x, r0.z};
    auto v1 = center + Vector2D{r1.x, r1.z};
    auto v2 = center + Vector2D{r2.x, r2.z};

    boat.position.y = waterHeight(waterSim, center);
    auto water_orientation = waterBuoyancyRotation(waterSim, v0, v1, v2);

    boat.transformation = Matrix4D::translation(boat.position) * water_orientation;
}
