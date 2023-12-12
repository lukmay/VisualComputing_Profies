#pragma once

#include "base.h"

#include <vector>

enum eDataIdx { Position = 0, Normal = 1, UV = 2 };

struct Vertex
{
    Vector3D pos;
    Vector3D normal;
    Vector2D uv;
};


struct Mesh
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    unsigned int size_vbo = 0;
    unsigned int size_ibo = 0;
};


/**
 * @brief Initializes all buffer objects (VBO, IBO) required for the mesh and fill it with data. Further, a vertex array
 * object (VAO) is created and the buffer objects are bind to it.
 *
 * @param vertices Data for each vertex of the mesh (position, color, normal and uv coordinate data).
 * @param indices List of indices that form polygons in the mesh.
 *
 * @return Initialized mesh structure that can be drawn with OpenGL.
 *
 * usage:
 *
 *   Mesh myMesh = meshCreate(vertex-data, index-data);
 *   glBindVertexArray(myMesh.vao);
 *   glDrawElements(GL_TRIANGLES, myMesh.size_ibo, GL_UNSIGNED_INT, nullptr);
 *
 */
Mesh meshCreate(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

/**
 * @brief Cleanup and delete all OpenGL buffers of a mesh. Has to be called for each mesh after it is not used anymore.
 *
 * @param mesh Mesh to delete.
 */
void meshDelete(const Mesh& mesh);
