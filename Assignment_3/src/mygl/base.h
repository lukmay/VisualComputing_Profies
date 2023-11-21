#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define to_degrees(x) (x * 180.0 / M_PI)
#define to_radians(x) (x * M_PI / 180.0)

#include <string>

#include "math/vector2d.h"
#include "math/vector3d.h"
#include "math/vector4d.h"
#include "math/matrix3d.h"
#include "math/matrix4d.h"


/**
 * @brief Create and initialize GLFW window and OpenGL context.
 *
 * @param title Window title
 * @param width Window width
 * @param height Window height
 *
 * @return Initialized GLFW window.
 */
GLFWwindow* windowCreate(const std::string &title, unsigned int width, unsigned int height);
/**
 * @brief Delete GLFW window and OpenGL contexst. Has to be called for each window after it is not used anymore.
 *
 * @param window GLFW window to delete.
 */
void windowDelete(GLFWwindow* window);

/**
 * @brief Save current viewport as PNG image.
 *
 * @param filepath Path to output image.
 */
void screenshotToPNG(const std::string &filepath);

/**
 * @brief Debugging function that checks for OpenGL errors and prints them if there are any.
 *
 * @param file Source file in which the error happend.
 * @param line Line in which the error happend.
 */
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)
