#include "framebuffer.h"

#include <cassert>
#include <stdexcept>
#include <iostream>

Framebuffer framebufferCreate(unsigned int width, unsigned int height)
{
    assert(width != 0 && height != 0);

    GLuint fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    /* create color attachment */
    GLuint colorTexture;
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();


    /* create depth/stencil attachment */
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();


    /* attach to framebuffer */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glCheckError();

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "[Framebuffer] is not a valid framebuffer (incomplete)!" << std::endl;
        std::cerr.flush();
        throw std::runtime_error("[Framebuffer] is not a valid framebuffer (incomplete)!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    return {fbo, colorTexture, depthTexture};
}

void framebufferDelete(const Framebuffer &fb)
{
    glDeleteTextures(1, &fb.colorTex);
    glDeleteTextures(1, &fb.depthTex);
    glDeleteFramebuffers(1, &fb.fbo);
    glCheckError();
}
