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

/* translation and scale for the scaled boad */
namespace body
{
const Vector4D color = {0.82, 0.41, 0.12, 1.0f};//R,G,B 210/255 105/255  30/255
const Matrix4D scale = Matrix4D::scale(3.5f, 0.9f, 1.25f);//x,z,y
const Matrix4D trans = Matrix4D::translation({0.0f, 0.9f, 0.0f});
}
namespace mast
{
const Vector4D color = {0.55, 0.27, 0.075, 1.0f};//R,G,B 139/255  69/255  19/255
const Matrix4D scale = Matrix4D::scale(0.15f, 1.5f, 0.15f);//x,z,y
const Matrix4D trans = Matrix4D::translation({1.0f, 3.3f, 0.0f});
}
namespace backblanke
{
const Vector4D color = {0.65, 0.16, 0.16, 1.0f};//R,G,B 165/255  42/255  42/255
const Matrix4D scale = Matrix4D::scale(0.15f, 0.3f, 1.25f);//x,z,y
const Matrix4D trans = Matrix4D::translation({-3.35f, 2.1f, 0.0f});
}
namespace frontblanke
{
const Matrix4D scale = Matrix4D::scale(0.15f, 0.3f, 1.25f);//x,z,y
const Matrix4D trans = Matrix4D::translation({3.35f, 2.1f, 0.0f});
}
namespace rigthblanke
{
const Matrix4D scale = Matrix4D::scale(3.2f, 0.3f, 0.15f);//x,z,y
const Matrix4D trans = Matrix4D::translation({0.0f, 2.1f, 1.1f});
}
namespace leftblanke
{
const Matrix4D scale = Matrix4D::scale(3.2f, 0.3f, 0.15f);//x,z,y
const Matrix4D trans = Matrix4D::translation({0.0f, 2.1f, -1.1f});
}
namespace bridge
{
const Vector4D color = {1.0, 1.0, 1.0, 1.0f};//R,G,B
const Matrix4D scale = Matrix4D::scale(0.65f, 0.9f, 0.375f);//x,z,y
const Matrix4D trans = Matrix4D::translation({-1.5f, 2.55f, 0.0f});
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

  /* body mesh and transformations */
  Mesh bodyMesh;
  Matrix4D bodyScalingMatrix;
  Matrix4D bodyTranslationMatrix;
  Matrix4D bodyTransformationMatrix;
  float bodySpinRadPerSecond;
  /*mast mesh transformation*/
  Mesh mastMesh;
  Matrix4D mastScalingMatrix;
  Matrix4D mastTranslationMatrix;
  Matrix4D mastTransformationMatrix;
  float mastSpinRadPerSecond;
  /* frontblank mesh and transformations */
  Mesh frontblankMesh;
  Matrix4D frontblankScalingMatrix;
  Matrix4D frontblankTranslationMatrix;
  Matrix4D frontblankTransformationMatrix;
  float frontblankSpinRadPerSecond;
  /*backblank mesh transformation*/
  Mesh backblankMesh;
  Matrix4D backblankScalingMatrix;
  Matrix4D backblankTranslationMatrix;
  Matrix4D backblankTransformationMatrix;
  float backblankSpinRadPerSecond;
  /* leftblanke mesh and transformations */
  Mesh leftblankeMesh;
  Matrix4D leftblankeScalingMatrix;
  Matrix4D leftblankeTranslationMatrix;
  Matrix4D leftblankeTransformationMatrix;
  float leftblankeSpinRadPerSecond;
  /*rightblanke mesh transformation*/
  Mesh rightblankeMesh;
  Matrix4D rightblankeScalingMatrix;
  Matrix4D rightblankeTranslationMatrix;
  Matrix4D rightblankeTransformationMatrix;
  float rightblankeSpinRadPerSecond;
  /*bridge mesh transformation*/
  Mesh bridgeMesh;
  Matrix4D bridgeScalingMatrix;
  Matrix4D bridgeTranslationMatrix;
  Matrix4D bridgeTransformationMatrix;
  float bridgeSpinRadPerSecond;

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
  sScene.bodyMesh = meshCreate(cube::vertexPos, cube::indices, body::color, GL_STATIC_DRAW, GL_STATIC_DRAW);
  sScene.mastMesh = meshCreate(cube::vertexPos, cube::indices, mast::color, GL_STATIC_DRAW, GL_STATIC_DRAW);
  sScene.backblankMesh = meshCreate(cube::vertexPos, cube::indices, backblanke::color, GL_STATIC_DRAW, GL_STATIC_DRAW);
  sScene.frontblankMesh = meshCreate(cube::vertexPos, cube::indices, backblanke::color, GL_STATIC_DRAW, GL_STATIC_DRAW);
  sScene.leftblankeMesh = meshCreate(cube::vertexPos, cube::indices, backblanke::color, GL_STATIC_DRAW, GL_STATIC_DRAW);
  sScene.rightblankeMesh = meshCreate(cube::vertexPos, cube::indices, backblanke::color, GL_STATIC_DRAW, GL_STATIC_DRAW);
  sScene.bridgeMesh = meshCreate(cube::vertexPos, cube::indices, bridge::color, GL_STATIC_DRAW, GL_STATIC_DRAW);
  sScene.water = waterCreate(waterPlane::color);
  //----------------------------------------------------------------------------------------------------------------------------------
  /* setup transformation matrices for objects */
  sScene.waterModelMatrix = waterPlane::trans;

  sScene.bodyScalingMatrix = body::scale;
  sScene.bodyTranslationMatrix = body::trans;
  sScene.bodyTransformationMatrix = Matrix4D::identity();

  sScene.mastScalingMatrix = mast::scale;
  sScene.mastTranslationMatrix = mast::trans;
  sScene.mastTransformationMatrix = Matrix4D::identity();

  sScene.backblankScalingMatrix = backblanke::scale;
  sScene.backblankTranslationMatrix = backblanke::trans;
  sScene.backblankTransformationMatrix = Matrix4D::identity();

  sScene.frontblankScalingMatrix = frontblanke::scale;
  sScene.frontblankTranslationMatrix = frontblanke::trans;
  sScene.frontblankTransformationMatrix = Matrix4D::identity();

  sScene.leftblankeScalingMatrix = leftblanke::scale;
  sScene.leftblankeTranslationMatrix = leftblanke::trans;
  sScene.leftblankeTransformationMatrix = Matrix4D::identity();

  sScene.rightblankeScalingMatrix = rigthblanke::scale;
  sScene.rightblankeTranslationMatrix = rigthblanke::trans;
  sScene.rightblankeTransformationMatrix = Matrix4D::identity();

  sScene.bridgeScalingMatrix = bridge::scale;
  sScene.bridgeTranslationMatrix = bridge::trans;
  sScene.bridgeTransformationMatrix = Matrix4D::identity();

  sScene.bodySpinRadPerSecond = M_PI / 2.0f;
  sScene.mastSpinRadPerSecond = M_PI / 2.0f;
  sScene.backblankSpinRadPerSecond = M_PI / 2.0f;
  sScene.frontblankSpinRadPerSecond = M_PI / 2.0f;
  sScene.leftblankeSpinRadPerSecond = M_PI / 2.0f;
  sScene.rightblankeSpinRadPerSecond = M_PI / 2.0f;
  sScene.bridgeSpinRadPerSecond = M_PI / 2.0f;


  /* load shader from file */
  sScene.shaderColor = shaderLoad("shader/default.vert", "shader/default.frag");
}
void wave(int index, float dt)
{
  for (size_t waveFuncIndex = 0; waveFuncIndex < 3; waveFuncIndex++)
  {
    sScene.water.vertices[index].pos.y +=
        sScene.waterSim.parameter[waveFuncIndex].amplitude* sin(sScene.waterSim.parameter[waveFuncIndex].omega *
                                                                             dot(Vector2D{sScene.water.vertices[index].pos.x, sScene.water.vertices[index].pos.z}, sScene.waterSim.parameter[waveFuncIndex].direction)
                                                                     + dt * sScene.waterSim.parameter[waveFuncIndex].phi
                                                                     );
  }
}
/*function to update waves of the water*/
void updateWater(float dt)
{
  sScene.waterSim.accumTime += dt;
  for (size_t i = 0; i < sScene.water.vertices.size(); i++)
  {
    sScene.water.vertices[i].pos.y = 0.0f;
    wave(i,sScene.waterSim.accumTime);
  }
  sScene.water.mesh = meshCreate(sScene.water.vertices, grid::indices, GL_DYNAMIC_DRAW, GL_STATIC_DRAW);
}
/* function to move and update objects in scene (e.g., rotate cube according to user input) */
void sceneUpdate(float dt)
{
  /* if 'w' or 's' pressed, cube should rotate around x axis */
  int movingDirX = 0;
  if (sInput.buttonPressed[0]) {
    movingDirX = -1;
  } else if (sInput.buttonPressed[1]) {
    movingDirX = 1;
  }

  /* if 'a' or 'd' pressed, cube should rotate around y axis */
  int rotationDirY = 0;
  if (sInput.buttonPressed[2]) {
    rotationDirY = 1;
  } else if (sInput.buttonPressed[3]) {
    rotationDirY = -1;
  }

  /* udpate cube transformation matrix to include new rotation if one of the keys was pressed  || */
  if (rotationDirY != 0) {
    sScene.bodyTransformationMatrix = Matrix4D::rotationY(rotationDirY * sScene.bodySpinRadPerSecond * dt) * Matrix4D::rotationX(movingDirX * sScene.bodySpinRadPerSecond * dt) * sScene.bodyTransformationMatrix;
    sScene.mastTranslationMatrix = Matrix4D::rotationY(rotationDirY * sScene.mastSpinRadPerSecond * dt) * Matrix4D::rotationX(movingDirX * sScene.mastSpinRadPerSecond * dt) * sScene.mastTranslationMatrix;
    sScene.bridgeTranslationMatrix = Matrix4D::rotationY(rotationDirY * sScene.bridgeSpinRadPerSecond * dt) * Matrix4D::rotationX(movingDirX * sScene.bridgeSpinRadPerSecond * dt) * sScene.bridgeTranslationMatrix;
    sScene.frontblankTranslationMatrix = Matrix4D::rotationY(rotationDirY * sScene.frontblankSpinRadPerSecond * dt) * Matrix4D::rotationX(movingDirX * sScene.frontblankSpinRadPerSecond * dt) * sScene.frontblankTranslationMatrix;
    sScene.backblankTranslationMatrix = Matrix4D::rotationY(rotationDirY * sScene.backblankSpinRadPerSecond * dt) * Matrix4D::rotationX(movingDirX * sScene.backblankSpinRadPerSecond * dt) * sScene.backblankTranslationMatrix;
    sScene.leftblankeTranslationMatrix = Matrix4D::rotationY(rotationDirY * sScene.leftblankeSpinRadPerSecond * dt) * Matrix4D::rotationX(movingDirX * sScene.leftblankeSpinRadPerSecond * dt) * sScene.leftblankeTranslationMatrix;
    sScene.rightblankeTranslationMatrix = Matrix4D::rotationY(rotationDirY * sScene.rightblankeSpinRadPerSecond * dt) * Matrix4D::rotationX(movingDirX * sScene.rightblankeSpinRadPerSecond * dt) * sScene.rightblankeTranslationMatrix;
    //------------------------------------------------------------------------------set other cubes
  }
  if(movingDirX != 0 && rotationDirY == 0){
    sScene.bodyTranslationMatrix = Matrix4D::translation(movingDirX * sScene.bodySpinRadPerSecond * dt) * sScene.bodyTranslationMatrix;
    sScene.mastTranslationMatrix = Matrix4D::translation(movingDirX * sScene.mastSpinRadPerSecond * dt) * sScene.mastTranslationMatrix;
    sScene.bridgeTranslationMatrix = Matrix4D::translation(movingDirX * sScene.bridgeSpinRadPerSecond * dt) * sScene.bridgeTranslationMatrix;
    sScene.frontblankTranslationMatrix = Matrix4D::translation(movingDirX * sScene.frontblankSpinRadPerSecond * dt) * sScene.frontblankTranslationMatrix;
    sScene.backblankTranslationMatrix = Matrix4D::translation(movingDirX * sScene.backblankSpinRadPerSecond * dt) * sScene.backblankTranslationMatrix;
    sScene.leftblankeTranslationMatrix = Matrix4D::translation(movingDirX * sScene.leftblankeSpinRadPerSecond * dt) * sScene.leftblankeTranslationMatrix;
    sScene.rightblankeTranslationMatrix = Matrix4D::translation(movingDirX * sScene.rightblankeSpinRadPerSecond * dt) * sScene.rightblankeTranslationMatrix;
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
    shaderUniform(sScene.shaderColor, "uModel", sScene.bodyTranslationMatrix * sScene.bodyTransformationMatrix * sScene.bodyScalingMatrix);
    glBindVertexArray(sScene.bodyMesh.vao);
    glDrawElements(GL_TRIANGLES, sScene.bodyMesh.size_ibo, GL_UNSIGNED_INT, nullptr);

    /*draw mast*/
    shaderUniform(sScene.shaderColor, "uModel", sScene.mastTranslationMatrix * sScene.mastTransformationMatrix * sScene.mastScalingMatrix);
    glBindVertexArray(sScene.mastMesh.vao);
    glDrawElements(GL_TRIANGLES, sScene.mastMesh.size_ibo, GL_UNSIGNED_INT, nullptr);

    /* draw backblanke */
    shaderUniform(sScene.shaderColor, "uModel", sScene.backblankTranslationMatrix * sScene.backblankTransformationMatrix * sScene.backblankScalingMatrix);
    glBindVertexArray(sScene.backblankMesh.vao);
    glDrawElements(GL_TRIANGLES, sScene.backblankMesh.size_ibo, GL_UNSIGNED_INT, nullptr);

    /*draw frontblanke*/
    shaderUniform(sScene.shaderColor, "uModel", sScene.frontblankTranslationMatrix * sScene.frontblankTransformationMatrix * sScene.frontblankScalingMatrix);
    glBindVertexArray(sScene.frontblankMesh.vao);
    glDrawElements(GL_TRIANGLES, sScene.frontblankMesh.size_ibo, GL_UNSIGNED_INT, nullptr);

    /* draw rigthblanke*/
    shaderUniform(sScene.shaderColor, "uModel", sScene.rightblankeTranslationMatrix * sScene.rightblankeTransformationMatrix * sScene.rightblankeScalingMatrix);
    glBindVertexArray(sScene.rightblankeMesh.vao);
    glDrawElements(GL_TRIANGLES, sScene.rightblankeMesh.size_ibo, GL_UNSIGNED_INT, nullptr);

    /*draw leftblanke*/
    shaderUniform(sScene.shaderColor, "uModel", sScene.leftblankeTranslationMatrix * sScene.leftblankeTransformationMatrix * sScene.leftblankeScalingMatrix);
    glBindVertexArray(sScene.leftblankeMesh.vao);
    glDrawElements(GL_TRIANGLES, sScene.leftblankeMesh.size_ibo, GL_UNSIGNED_INT, nullptr);

    /* draw cabine*/
    shaderUniform(sScene.shaderColor, "uModel", sScene.bridgeTranslationMatrix * sScene.bridgeTransformationMatrix * sScene.bridgeScalingMatrix);
    glBindVertexArray(sScene.bridgeMesh.vao);
    glDrawElements(GL_TRIANGLES, sScene.bridgeMesh.size_ibo, GL_UNSIGNED_INT, nullptr);

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
  int iteration = 0;
  /* loop until user closes window */
  while(!glfwWindowShouldClose(window))
  {
    /* poll and process input and window events */
    glfwPollEvents();

    /* update model matrix of cube */
    timeStampNew = glfwGetTime();
    if(iteration < 1000)        // TODO : Remove this when implementation is finished.
    {
      updateWater(timeStampNew - timeStamp);
      sceneUpdate(timeStampNew - timeStamp);
      timeStamp = timeStampNew;
      iteration++;
    }

    /* draw all objects in the scene */
    sceneDraw();

    /* swap front and back buffer */
    glfwSwapBuffers(window);
  }


  /*-------- cleanup --------*/
  /* delete opengl shader and buffers */
  shaderDelete(sScene.shaderColor);
  waterDelete(sScene.water);
  meshDelete(sScene.bodyMesh);
  meshDelete(sScene.backblankMesh);
  meshDelete(sScene.mastMesh);
  meshDelete(sScene.frontblankMesh);
  meshDelete(sScene.leftblankeMesh);
  meshDelete(sScene.rightblankeMesh);
  meshDelete(sScene.bridgeMesh);
  //------------------------------------------------------------------------------set other cubes

  /* cleanup glfw/glcontext */
  windowDelete(window);

  return EXIT_SUCCESS;
}
