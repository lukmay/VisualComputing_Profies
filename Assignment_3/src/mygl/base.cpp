#include "base.h"

#include <iostream>
#include <sstream>
#include <vector>

#include <stb_image/stb_image_write.h>

/**
 * debugging function from Joey de Vries (LearnOpenGL)
 * https://learnopengl.com/In-Practice/Debugging
**/
GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cerr << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

void screenshotToPNG(const std::string &filepath)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int width  = viewport[2];
    int height = viewport[3];
    int nPixels = width * height;

    std::vector<GLubyte> data(4 * nPixels);

    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath.c_str(), width, height, 4, data.data(), width * 4);
}

void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error: " <<  description << std::endl;
}

GLFWwindow* windowCreate(const std::string& title, unsigned int width = 1280, unsigned int height = 720)
{
    /*-------------- init glfw ----------------*/
    if(!glfwInit())
    {
        std::cerr << "Couldn't initialize GLFW" << std::endl;
        return nullptr;
    }

    /* set callback function for glfw error */
    glfwSetErrorCallback(glfw_error_callback);


    /*-------------- create window ----------------*/
    /* set window hints */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* create window and its opengl context */
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(window == nullptr)
    {
        std::cerr << "Couldn't create Window (GL context)" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    /* make context the current one */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /*-------------- init glad ----------------*/
    /* load opengl extensions */
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Couldn't initialize GLAD" << std::endl;
        windowDelete(window);
        return nullptr;
    }

    return window;
}


void windowDelete(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
