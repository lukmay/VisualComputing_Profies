#pragma once

#include "mygl/base.h"
#include "mygl/mesh.h"

struct WaveParams
{
    float amplitude;
    float phi;
    float omega;
    Vector2D direction;
};

struct WaterSim
{
    /**
     * Parameters for the 3 wave functions for the water simulation
     */
    WaveParams parameter[3] =
    {
        { 0.6f,  0.5f,  0.25f, normalize(Vector2D{1.0f,  1.0f}) },
        { 0.7f,  0.25f, 0.1f,  normalize(Vector2D{1.0f, -1.0f}) },
        { 0.1f,  0.9f,  0.9f,  normalize(Vector2D{-1.0f, 0.0f}) },
    };

    float accumTime = 0.0f;
};

struct Water
{
    Mesh mesh;
    std::vector<Vertex> vertices;
};

/**
 * @brief Initializes plane grid to visualize water surface. For that a vector containing all grid vertices is created and
 * a mesh (see function meshCreate(...)) is setup with these vertices.
 *
 * @param color Base color of water surface.
 *
 * @return Object containing the vector of vertices and an initialized mesh structure that can be drawn with OpenGL.
 *
 * usage:
 *
 *   Water myWater = waterCreate({0.0, 0.0, 1.0, 0.5})
 *   glBindVertexArray(myWater.mesh.vao);
 *   glDrawElements(GL_TRIANGLES, myWater.size_ibo, GL_UNSIGNED_INT, nullptr);
 *
 */
Water waterCreate(const Vector4D &color);

/**
 * @brief Cleanup and delete all OpenGL buffers of the water mesh. Has to be called for each water after it is not used anymore.
 *
 * @param water Water to delete.
 */
void waterDelete(Water& water);
