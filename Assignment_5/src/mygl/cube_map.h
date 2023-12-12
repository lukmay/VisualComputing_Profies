#pragma once

#include "base.h"

#include <vector>
#include <array>

enum eDataIdxCubeMap { PositionCubeMap = 0 };

struct MeshCubeMap
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    unsigned int size_vbo = 0;
    unsigned int size_ibo = 0;
};


MeshCubeMap meshCubeMapCreate(const std::vector<Vector3D>& vertices, const std::vector<unsigned int>& indices);

/**
 * @brief Cleanup and delete all OpenGL buffers of a mesh. Has to be called for each mesh after it is not used anymore.
 *
 * @param mesh Mesh to delete.
 */
void meshCubeMapDelete(const MeshCubeMap& mesh);


struct TextureCube
{
    GLuint id = 0;

    unsigned int width = 0;
    unsigned int height = 0;
};

/**
 * @brief Initialize OpenGL cube map texture and load it from file.
 *
 * @param path Path to folder containing texture files. They need to be 
 *
 * @return Initialized texture object.
 */
TextureCube textureCubeLoad(const std::array<std::string, 6>& image_paths);
/**
 * @brief Delete texture cube object. Has to be called for each texture after it is not used anymore.
 *
 * @param texture Texture cube to delete.
 */
void textureCubeDelete(const TextureCube& texture);

struct CubeMap
{
    MeshCubeMap mesh;
    TextureCube texture;
};

CubeMap cubeMapCreate(const std::vector<Vector3D>& vertices, const std::vector<unsigned int>& indices, const std::array<std::string, 6>& image_paths);

void cubeMapDelete(const CubeMap& cubeMap);
