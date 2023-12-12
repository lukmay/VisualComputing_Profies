#pragma once

#include "base.h"

struct Texture
{
    GLuint id = 0;

    unsigned int width = 0;
    unsigned int height = 0;
};

/**
 * @brief Initialize OpenGL texture and load it from file.
 *
 * @param path Path to texture file.
 *
 * @return Initialized texture object.
 */
Texture textureLoad(const std::string& path);
/**
 * @brief Delete texture object. Has to be called for each texture after it is not used anymore.
 *
 * @param texture Texture to delete.
 */
void textureDelete(const Texture& texture);
