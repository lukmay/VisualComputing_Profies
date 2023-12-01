#pragma once

#include "base.h"

struct Framebuffer
{
    GLuint fbo = 0;
    GLuint colorTex = 0;
    GLuint depthTex = 0;
};

/**
 * @brief Initializes framebuffer with color and depth textures.
 *
 * @param width Framebuffer width.
 * @param height Framebuffer height.
 *
 * @return Initialized framebuffer object.
 */
Framebuffer framebufferCreate(unsigned int width, unsigned int height);
/**
 * @brief Cleanup and delete all OpenGL buffers of a framebuffer. Has to be called for each framebuffer after it is not used anymore.
 *
 * @param fb Framebuffer to delete.
 */
void framebufferDelete(const Framebuffer& fb);
