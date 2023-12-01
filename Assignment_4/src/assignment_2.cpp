#include <cstdlib>
#include <iostream>

#include "mygl/shader.h"
#include "mygl/model.h"
#include "mygl/camera.h"

#include "boat.h"
#include "water.h"

struct
{
    Camera camera;
    bool cameraFollowBoat;
    float zoomSpeedMultiplier;

    Boat boat;
    Model water;

    ShaderProgram shaderBoat;
    ShaderProgram shaderWater;

    WaterSim waterSim;
} sScene;

struct
{
    bool mouseButtonPressed = false;
    Vector2D mousePressStart;
    bool keyPressed[Boat::eControl::CONTROL_COUNT] = {false, false, false, false};
} sInput;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /* input for camera control */
    if(key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        sScene.cameraFollowBoat = false;
        sScene.camera.lookAt = {0.0f, 0.0f, 0.0f};
        cameraUpdateOrbit(sScene.camera, {0.0f, 0.0f}, 0.0f);
    }
    if(key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        sScene.cameraFollowBoat = false;
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        sScene.cameraFollowBoat = true;
    }

    /* input for boat control */
    if(key == GLFW_KEY_W)
    {
        sInput.keyPressed[Boat::eControl::THROTTLE_UP] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    if(key == GLFW_KEY_S)
    {
        sInput.keyPressed[Boat::eControl::THROTTLE_DOWN] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }

    if(key == GLFW_KEY_A)
    {
        sInput.keyPressed[Boat::eControl::RUDDER_LEFT] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    if(key == GLFW_KEY_D)
    {
        sInput.keyPressed[Boat::eControl::RUDDER_RIGHT] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }

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
}

void mousePosCallback(GLFWwindow* window, double x, double y)
{
    if(sInput.mouseButtonPressed)
    {
        Vector2D diff = sInput.mousePressStart - Vector2D(x, y);
        cameraUpdateOrbit(sScene.camera, diff, 0.0f);
        sInput.mousePressStart = Vector2D(x, y);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        sInput.mouseButtonPressed = (action == GLFW_PRESS);

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        sInput.mousePressStart = Vector2D(x, y);
    }
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    cameraUpdateOrbit(sScene.camera, {0, 0}, sScene.zoomSpeedMultiplier * yoffset);
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    sScene.camera.width = width;
    sScene.camera.height = height;
}

void sceneInit(float width, float height)
{
    sScene.camera = cameraCreate(width, height, to_radians(45.0), 0.01, 500.0, {10.0, 10.0, 10.0}, {0.0, 0.0, 0.0});
    sScene.cameraFollowBoat = true;
    sScene.zoomSpeedMultiplier = 0.05f;

    sScene.boat = boatLoad("assets/boat/boat.obj");
    sScene.water = modelLoad("assets/water/water.obj").front();

    sScene.shaderBoat = shaderLoad("shader/default.vert", "shader/color.frag");
    sScene.shaderWater = shaderLoad("shader/default.vert", "shader/color.frag");
}

void sceneUpdate(float dt)
{
    sScene.waterSim.accumTime += dt;

    boatMove(sScene.boat, sScene.waterSim, sInput.keyPressed, dt);

    if (!sScene.cameraFollowBoat)
        cameraFollow(sScene.camera, sScene.boat.position);
}

void render()
{
    /* setup camera and model matrices */
    Matrix4D proj = cameraProjection(sScene.camera);
    Matrix4D view = cameraView(sScene.camera);
    glUseProgram(sScene.shaderBoat.id);
    shaderUniform(sScene.shaderBoat, "uProj",  proj);
    shaderUniform(sScene.shaderBoat, "uView",  view);
    shaderUniform(sScene.shaderBoat, "uModel",  sScene.boat.transformation);

    for(unsigned int i = 0; i < sScene.boat.partModel.size(); i++)
    {
        auto& model = sScene.boat.partModel[i];
        glBindVertexArray(model.mesh.vao);

        shaderUniform(sScene.shaderBoat, "uModel", sScene.boat.transformation);

        for(auto& material : model.material)
        {
            /* set material properties */
            shaderUniform(sScene.shaderBoat, "uMaterial.diffuse", material.diffuse);

            glDrawElements(GL_TRIANGLES, material.indexCount, GL_UNSIGNED_INT, (const void*) (material.indexOffset*sizeof(unsigned int)) );
        }
    }

    /* render water */
    {
        glUseProgram(sScene.shaderWater.id);

        /* setup camera and model matrices */
        shaderUniform(sScene.shaderWater, "uProj",  proj);
        shaderUniform(sScene.shaderWater, "uView",  view);
        shaderUniform(sScene.shaderWater, "uModel", Matrix4D::identity());

        /* set material properties */
        shaderUniform(sScene.shaderWater, "uMaterial.diffuse", sScene.water.material.front().diffuse);

        glBindVertexArray(sScene.water.mesh.vao);
        glDrawElements(GL_TRIANGLES, sScene.water.material.front().indexCount, GL_UNSIGNED_INT, (const void*) (sScene.water.material.front().indexOffset*sizeof(unsigned int)) );
    }

    /* cleanup opengl state */
    glBindVertexArray(0);
    glUseProgram(0);
}


void sceneDraw()
{
    glClearColor(135.0 / 255, 206.0 / 255, 235.0 / 255, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*------------ render scene -------------*/
    render();

    /* cleanup opengl state */
    glBindVertexArray(0);
    glUseProgram(0);
}

int main(int argc, char** argv)
{
    /*---------- init window ------------*/
    int width = 1280;
    int height = 720;
    GLFWwindow* window = windowCreate("Assignment 2 - Shader Programming", width, height);
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
    while(!glfwWindowShouldClose(window))
    {
        /* poll and process input and window events */
        glfwPollEvents();

        /* update scene */
        timeStampNew = glfwGetTime();
        sceneUpdate(timeStampNew - timeStamp);
        timeStamp = timeStampNew;

        /* draw all objects in the scene */
        sceneDraw();

        /* swap front and back buffer */
        glfwSwapBuffers(window);
    }


    /*-------- cleanup --------*/
    boatDelete(sScene.boat);
    modelDelete(sScene.water);
    shaderDelete(sScene.shaderBoat);
    shaderDelete(sScene.shaderWater);
    windowDelete(window);

    return EXIT_SUCCESS;
}
