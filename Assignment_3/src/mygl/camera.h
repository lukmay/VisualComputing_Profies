#pragma once

#include <math/vector2d.h>
#include <math/vector3d.h>
#include <math/matrix4d.h>

struct Camera
{
    float width;
    float height;
    float fov;
    float nearPlane;
    float farPlane;
    Vector3D position;
    Vector3D lookAt;
    Vector3D initUp;
};

/**
 * @brief Function to initialize a camera.
 *
 * @param width Image width.
 * @param height Image height.
 * @param fov Field of view (in rad).
 * @param nearPlane Plane defining closest points that get rendered.
 * @param farPlane Plane defining fardest points that get rendered.
 * @param initPos Initial position of camera.
 * @param lookAt Point at which the camera is looking at.
 * @param initUp Initial up direction of camera.
 *
 * @return Initialized camera object.
 */
Camera cameraCreate(float width, float height, float fov, float nearPlane, float farPlane, const Vector3D& initPos, const Vector3D& lookAt = {0, 0, 0}, const Vector3D& initUp = {0, 1, 0});

/**
 * @brief Get projection matrix from a camera.
 *
 * @param cam Camera from which the projection matrix is calculated.
 *
 * @return Projection matrix.
 */
Matrix4D cameraProjection(const Camera& cam);

/**
 * @brief Get view matrix from a camera.
 *
 * @param cam Camera from which the view matrix is calculated.
 *
 * @return View matrix.
 */
Matrix4D cameraView(const Camera& cam);

/**
 * @brief Update camera position on the orbit around the look at point using spherical coordinates.
 *
 * @param cam Camera that gets updated.
 * @param mouseDiff X and Y distances that are used to change the spherical coordinate angles of the camera position.
 * with respect to the look at point.
 * @param zoom Factor to zoom in (-) or out (+) (distance of camera position to look at point is de-/increased).
 */
void cameraUpdateOrbit(Camera &cam, const Vector2D &mouseDiff, float zoom);
