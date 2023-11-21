#include <cstdlib>
#include <iostream>

#include "mygl/shader.h"
#include "mygl/mesh.h"
#include "mygl/geometry.h"
#include "mygl/camera.h"
#include "water.h"

/* translation and color for the water plane */
namespace waterPlane
{
const Vector4D color = {0.0f, 0.0f, 0.35f, 1.0f};
const Matrix4D trans = Matrix4D::identity();
}

/* translation and scale for the scaled cube */
namespace scaledCube
{
const Matrix4D scale = Matrix4D::scale(2.0f, 2.0f, 2.0f);
const Matrix4D trans = Matrix4D::translation({0.0f, 4.0f, 0.0f});
}

/* struct holding all necessary state variables for scene */
struct
{
    /* camera */
    Camera camera;
    float zoomSpeedMultiplier;

    /* water */
    WaterSim waterSim;
    Water water;
    Matrix4D waterModelMatrix;

    /* cube mesh and transformations */
    Mesh cubeMesh;
    Matrix4D cubeScalingMatrix;
    Matrix4D cubeTranslationMatrix;
    Matrix4D cubeTransformationMatrix;
    float cubeSpinRadPerSecond;

    /* shader */
    ShaderProgram shaderColor;
} sScene;

/* struct holding all state variables for input */
struct
{
    bool mouseLeftButtonPressed = false;
    Vector2D mousePressStart;
    bool buttonPressed[4] = {false, false, false, false};
} sInput;

/* GLFW callback function for keyboard events */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /* called on keyboard event */

    /* close window on escape */
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    /* make screenshot and save in work directory */
    if(key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        screenshotToPNG("screenshot.png");
    }

    /* input for cube control */
    if(key == GLFW_KEY_W)
    {
        sInput.buttonPressed[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    if(key == GLFW_KEY_S)
    {
        sInput.buttonPressed[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }

    if(key == GLFW_KEY_A)
    {
        sInput.buttonPressed[2] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    if(key == GLFW_KEY_D)
    {
        sInput.buttonPressed[3] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
}

/* GLFW callback function for mouse position events */
void mousePosCallback(GLFWwindow* window, double x, double y)
{
    /* called on cursor position change */
    if(sInput.mouseLeftButtonPressed)
    {
        Vector2D diff = sInput.mousePressStart - Vector2D(x, y);
        cameraUpdateOrbit(sScene.camera, diff, 0.0f);
        sInput.mousePressStart = Vector2D(x, y);
    }
}

/* GLFW callback function for mouse button events */
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        sInput.mouseLeftButtonPressed = (action == GLFW_PRESS || action == GLFW_REPEAT);

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        sInput.mousePressStart = Vector2D(x, y);
    }
}

/* GLFW callback function for mouse scroll events */
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    cameraUpdateOrbit(sScene.camera, {0, 0}, sScene.zoomSpeedMultiplier * yoffset);
}

/* GLFW callback function for window resize event */
void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    sScene.camera.width = width;
    sScene.camera.height = height;
}

/* function to setup and initialize the whole scene */
void sceneInit(float width, float height)
{
    /* initialize camera */
    sScene.camera = cameraCreate(width, height, to_radians(45.0f), 0.01f, 500.0f, {10.0f, 14.0f, 10.0f}, {0.0f, 4.0f, 0.0f});
    sScene.zoomSpeedMultiplier = 0.05f;

    /* setup objects in scene and create opengl buffers for meshes */
    sScene.cubeMesh = meshCreate(cube::vertices, cube::indices, GL_STATIC_DRAW, GL_STATIC_DRAW);
    sScene.water = waterCreate(waterPlane::color);

    /* setup transformation matrices for objects */
    sScene.waterModelMatrix = waterPlane::trans;

    sScene.cubeScalingMatrix = scaledCube::scale;
    sScene.cubeTranslationMatrix = scaledCube::trans;

    sScene.cubeTransformationMatrix = Matrix4D::identity();

    sScene.cubeSpinRadPerSecond = M_PI / 2.0f;

    /* load shader from file */
    sScene.shaderColor = shaderLoad("shader/default.vert", "shader/default.frag");
}

/* function to move and update objects in scene (e.g., rotate cube according to user input) */
void sceneUpdate(float dt)
{
    /* if 'w' or 's' pressed, cube should rotate around x axis */
    int rotationDirX = 0;
    if (sInput.buttonPressed[0]) {
        rotationDirX = -1;
    } else if (sInput.buttonPressed[1]) {
        rotationDirX = 1;
    }

    /* if 'a' or 'd' pressed, cube should rotate around y axis */
    int rotationDirY = 0;
    if (sInput.buttonPressed[2]) {
        rotationDirY = -1;
    } else if (sInput.buttonPressed[3]) {
        rotationDirY = 1;
    }

    /* udpate cube transformation matrix to include new rotation if one of the keys was pressed */
    if (rotationDirX != 0 || rotationDirY != 0) {
        sScene.cubeTransformationMatrix = Matrix4D::rotationY(rotationDirY * sScene.cubeSpinRadPerSecond * dt) * Matrix4D::rotationX(rotationDirX * sScene.cubeSpinRadPerSecond * dt) * sScene.cubeTransformationMatrix;
    }
}

/* function to draw all objects in the scene */
void sceneDraw()
{
    /* clear framebuffer color */
    glClearColor(135.0 / 255, 206.0 / 255, 235.0 / 255, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*------------ render scene -------------*/
    /* use shader and set the uniforms (names match the ones in the shader) */
    {
        glUseProgram(sScene.shaderColor.id);
        shaderUniform(sScene.shaderColor, "uProj",  cameraProjection(sScene.camera));
        shaderUniform(sScene.shaderColor, "uView",  cameraView(sScene.camera));

        /* draw water plane */
        shaderUniform(sScene.shaderColor, "uModel", sScene.waterModelMatrix);
        glBindVertexArray(sScene.water.mesh.vao);
        glDrawElements(GL_TRIANGLES, sScene.water.mesh.size_ibo, GL_UNSIGNED_INT, nullptr);

        /* draw cube, requires to calculate the final model matrix from all transformations */
        shaderUniform(sScene.shaderColor, "uModel", sScene.cubeTranslationMatrix * sScene.cubeTransformationMatrix * sScene.cubeScalingMatrix);
        glBindVertexArray(sScene.cubeMesh.vao);
        glDrawElements(GL_TRIANGLES, sScene.cubeMesh.size_ibo, GL_UNSIGNED_INT, nullptr);
    }
    glCheckError();

    /* cleanup opengl state */
    glBindVertexArray(0);
    glUseProgram(0);
}

int main(int argc, char** argv)
{
    /* create window/context */
    int width = 1280;
    int height = 720;
    GLFWwindow* window = windowCreate("Assignment 1 - Transformations, User Input and Camera", width, height);
    if(!window) { return EXIT_FAILURE; }

    /* set window callbacks */
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);


    /*---------- init opengl stuff ------------*/
    glEnable(GL_DEPTH_TEST);

    /* setup scene */
    sceneInit(width, height);

    /*-------------- main loop ----------------*/
    double timeStamp = glfwGetTime();
    double timeStampNew = 0.0;

    /* loop until user closes window */
    while(!glfwWindowShouldClose(window))
    {
        /* poll and process input and window events */
        glfwPollEvents();

        /* update model matrix of cube */
        timeStampNew = glfwGetTime();
        sceneUpdate(timeStampNew - timeStamp);
        timeStamp = timeStampNew;

        /* draw all objects in the scene */
        sceneDraw();

        /* swap front and back buffer */
        glfwSwapBuffers(window);
    }


    /*-------- cleanup --------*/
    /* delete opengl shader and buffers */
    shaderDelete(sScene.shaderColor);
    waterDelete(sScene.water);
    meshDelete(sScene.cubeMesh);

    /* cleanup glfw/glcontext */
    windowDelete(window);

    return EXIT_SUCCESS;
}
