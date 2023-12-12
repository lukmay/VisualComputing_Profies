#include "texture.h"

#include <stdexcept>
#include <iostream>

#include <stb_image/stb_image.h>

Texture textureLoad(const std::string &path)
{
    int width = 0, height = 0, components = 0;

    /* flip image to match opengl's texture coordinates */
    stbi_set_flip_vertically_on_load(true);

    /* load image */
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &components, 4);
    if(data == nullptr)
    {
        std::cerr << "[Texture] couldn't load image file " << path << std::endl;
        std::cerr.flush();
        throw std::runtime_error("[Texture] couldn't load image file " + path);
    }

    /* upload data */
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glCheckError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture{id, (unsigned int) width, (unsigned int) height};
}

void textureDelete(const Texture &texture)
{
    glDeleteTextures(1, &texture.id);
}
