#include "camera.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

namespace detail
{

Vector3D sphericalCoords(const Camera& cam)
{
    Vector3D cartVec = cam.position - cam.lookAt;

    auto r = length(cartVec);
    auto phi = atan2(cartVec.x, cartVec.z);
    auto theta = atan2(sqrt(cartVec.x * cartVec.x + cartVec.z * cartVec.z), cartVec.y);

    return Vector3D(r, phi, theta);
}

}

Camera cameraCreate(float width, float height, float fov, float nearPlane, float farPlane, const Vector3D &initPos, const Vector3D &lookAt, const Vector3D &initUp)
{
    return {width, height, fov, nearPlane, farPlane, initPos, lookAt, initUp};
}

Matrix4D cameraProjection(const Camera &cam)
{
    return Matrix4D::perspective(cam.fov, cam.width/cam.height, cam.nearPlane, cam.farPlane);
}

Matrix4D cameraView(const Camera &cam)
{
    Vector3D front = normalize(cam.lookAt - cam.position);
    Vector3D right = normalize(cross(front, cam.initUp));
    Vector3D up = normalize(cross(right, front));

    Matrix4D rotation(
             right.x,     right.y,     right.z,     0.0f,
             up.x,       up.y,       up.z,       0.0f,
            -front.x,   -front.y,   -front.z,    0.0f,
             0.0f,       0.0f,       0.0f,       1.0f
            );

    return  rotation * Matrix4D::translation(-cam.position);
} 

void cameraUpdateOrbit(Camera& cam, const Vector2D& mouseDiff, float zoom)
{
    Vector3D spherCoord = detail::sphericalCoords(cam);
    float r = spherCoord[0];
    float phi = spherCoord[1];
    float theta = spherCoord[2];

    phi += mouseDiff.x * (M_PI / cam.width);
    theta += mouseDiff.y * (M_PI / cam.height);
    r += zoom * r;

    theta = std::clamp<float>(theta, 1e-4, M_PI - 1e-4);
    r = std::max(r, 1e-4f);

    Vector3D cartCoord(r * sin(theta) * sin(phi), r * cos(theta), r * sin(theta) * cos(phi));

    cam.position = cam.lookAt + cartCoord;
}
