#include <cstdlib>
#include <iostream>

#include "mygl/camera.h"
#include "mygl/light.h"
#include "mygl/model.h"
#include "mygl/shader.h"

#include "boat.h"
#include "water.h"

struct {
  Camera camera;
  bool cameraFollowBoat;
  float zoomSpeedMultiplier;

  Boat boat;
  Model water;
  Light light;
  bool isNight;
  ShaderProgram shaderBoat;
  ShaderProgram shaderWater;

  WaterSim waterSim;
} sScene;

struct {
  bool mouseButtonPressed = false;
  Vector2D mousePressStart;
  bool keyPressed[Boat::eControl::CONTROL_COUNT] = {false, false, false, false};
} sInput;

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  /* input for camera control */
  if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
    sScene.cameraFollowBoat = false;
    sScene.camera.lookAt = {0.0f, 0.0f, 0.0f};
    cameraUpdateOrbit(sScene.camera, {0.0f, 0.0f}, 0.0f);
  }
  if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
    sScene.cameraFollowBoat = false;
  }
  if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
    sScene.cameraFollowBoat = true;
  }

  /* input for boat control */
  if (key == GLFW_KEY_W) {
    sInput.keyPressed[Boat::eControl::THROTTLE_UP] =
        (action == GLFW_PRESS || action == GLFW_REPEAT);
  }
  if (key == GLFW_KEY_S) {
    sInput.keyPressed[Boat::eControl::THROTTLE_DOWN] =
        (action == GLFW_PRESS || action == GLFW_REPEAT);
  }

  if (key == GLFW_KEY_A) {
    sInput.keyPressed[Boat::eControl::RUDDER_LEFT] =
        (action == GLFW_PRESS || action == GLFW_REPEAT);
  }
  if (key == GLFW_KEY_D) {
    sInput.keyPressed[Boat::eControl::RUDDER_RIGHT] =
        (action == GLFW_PRESS || action == GLFW_REPEAT);
  }

  /* close window on escape */
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  /* make screenshot and save in work directory */
  if (key == GLFW_KEY_P && action == GLFW_PRESS) {
    screenshotToPNG("screenshot.png");
  }

  /* control to switch between day and night */
  if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
    sScene.isNight = false;
    setDayLight(sScene.light);
  }
  if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
    sScene.isNight = true;
    setNightLight(sScene.light);
  }
}

void mousePosCallback(GLFWwindow *window, double x, double y) {
  if (sInput.mouseButtonPressed) {
    Vector2D diff = sInput.mousePressStart - Vector2D(x, y);
    cameraUpdateOrbit(sScene.camera, diff, 0.0f);
    sInput.mousePressStart = Vector2D(x, y);
  }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    sInput.mouseButtonPressed = (action == GLFW_PRESS);

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    sInput.mousePressStart = Vector2D(x, y);
  }
}

void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  cameraUpdateOrbit(sScene.camera, {0, 0},
                    sScene.zoomSpeedMultiplier * yoffset);
}

void windowResizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  sScene.camera.width = width;
  sScene.camera.height = height;
}

void sceneInit(float width, float height) {
  sScene.camera = cameraCreate(width, height, to_radians(45.0), 0.01, 500.0,
                               {40.0, 3.0, 0.0}, {0.0, 0.0, 0.0});
  sScene.cameraFollowBoat = true;
  sScene.zoomSpeedMultiplier = 0.05f;

  sScene.light = createLight();
  sScene.isNight = false;

  sScene.boat = boatLoad("assets/boat/boat.obj");
  sScene.water = modelLoad("assets/water/water.obj").front();

  sScene.shaderBoat = shaderLoad("shader/default.vert", "shader/color.frag");
  sScene.shaderWater = shaderLoad("shader/water.vert", "shader/color.frag");
}

void sceneUpdate(float dt) {
  sScene.waterSim.accumTime += dt;

  boatMove(sScene.boat, sScene.waterSim, sInput.keyPressed, dt);

  if (!sScene.cameraFollowBoat)
    cameraFollow(sScene.camera, sScene.boat.position);
}
void render() {
  /* setup camera and model matrices */
  Matrix4D proj = cameraProjection(sScene.camera);
  Matrix4D view = cameraView(sScene.camera);
  glUseProgram(sScene.shaderBoat.id);
  shaderUniform(sScene.shaderBoat, "uProj", proj);
  shaderUniform(sScene.shaderBoat, "uView", view);
  shaderUniform(sScene.shaderBoat, "uModel", sScene.boat.transformation);
  shaderUniform(sScene.shaderBoat, "uCamera.position", sScene.camera.position);
  /* Set shader for light */
  shaderUniform(sScene.shaderBoat, "uLight.ambientColor",
                sScene.light.ambientColor);
  shaderUniform(sScene.shaderBoat, "uLight.color", sScene.light.color);
  shaderUniform(sScene.shaderBoat, "uLight.direction", sScene.light.direction);
  shaderUniform(sScene.shaderBoat, "uLight.ambientCoeff",
                sScene.light.ambientCoeff);
  shaderUniform(sScene.shaderBoat, "uLight.diffuseCoeff",
                sScene.light.diffuseCoeff);
  shaderUniform(sScene.shaderBoat, "uLight.specularCoeff",
                sScene.light.specularCoeff);

  Matrix3D lightDirectionRot1 = Matrix3D(cos(M_PI_2f), 0, sin(M_PI_2f), 0, 1, 0,
                                         -sin(M_PI_2f), 0, cos(M_PI_2f));

  Matrix3D lightDirectionRot2 = Matrix3D(cos(M_PI_2f), 0, sin(M_PI_2f), 0, 1, 0,
                                         -sin(M_PI_2f), 0, cos(M_PI_2f));

  Matrix3D lightDirectionRot3 =
      Matrix3D(cos(M_PIf), 0, sin(M_PIf), 0, 1, 0, -sin(M_PI), 0, cos(M_PIf));

  /* frontlight 1 */
  shaderUniform(sScene.shaderBoat, "uFrontLight1.color",
                Vector3D{1.0f, 1.0f, 1.0f});
  shaderUniform(sScene.shaderBoat, "uFrontLight1.direction",
                normalize(lightDirectionRot1 *
                          Vector3D{sScene.boat.transformation[0][0],
                                   sScene.boat.transformation[0][1] - 0.6f,
                                   sScene.boat.transformation[0][2]}));
  shaderUniform(sScene.shaderBoat, "uFrontLight1.position",
                Vector3D{sScene.boat.position.x + 1.0f,
                         sScene.boat.position.y + 1.0f,
                         sScene.boat.position.z});
  shaderUniform(sScene.shaderBoat, "uFrontLight1.cutOff",
                cos(75.0f * M_PIf / 180));

  /* frontlight 2 */
  shaderUniform(sScene.shaderBoat, "uFrontLight2.color",
                Vector3D{1.0f, 1.0f, 1.0f});
  shaderUniform(sScene.shaderBoat, "uFrontLight2.direction",
                normalize(lightDirectionRot2 *
                          Vector3D{sScene.boat.transformation[0][0],
                                   sScene.boat.transformation[0][1] - 0.6f,
                                   sScene.boat.transformation[0][2]}));
  shaderUniform(sScene.shaderBoat, "uFrontLight2.position",
                Vector3D{sScene.boat.position.x - 1.0f,
                         sScene.boat.position.y + 1.0f,
                         sScene.boat.position.z});
  shaderUniform(sScene.shaderBoat, "uFrontLight2.cutOff",
                cos(75.0f * M_PIf / 180));

  /* red light on left-hand side of boat */
  shaderUniform(sScene.shaderBoat, "uRedLight.color",
                Vector3D{0.53f, 0.04f, 0.04f});
  shaderUniform(sScene.shaderBoat, "uRedLight.direction",
                lightDirectionRot3 *
                    Vector3D(sScene.boat.transformation[0][0],
                             sScene.boat.transformation[0][1] - 0.5f,
                             sScene.boat.transformation[0][2]));
  shaderUniform(sScene.shaderBoat, "uRedLight.position",
                Vector3D{sScene.boat.position.x - 0.7f,
                         sScene.boat.position.y + 1.0f,
                         sScene.boat.position.z});
  shaderUniform(sScene.shaderBoat, "uGreenLight.cutOff",
                cos(10.0f * M_PIf / 180));

  /* green light on the right-hand side */
  shaderUniform(sScene.shaderBoat, "uGreenLight.color",
                Vector3D{0.0f, 0.39f, 0.0f});
  shaderUniform(sScene.shaderBoat, "uGreenLight.direction",
                Vector3D(sScene.boat.transformation[0][0],
                         sScene.boat.transformation[0][1] - 0.5f,
                         sScene.boat.transformation[0][2]));
  shaderUniform(sScene.shaderBoat, "uGreenLight.position",
                Vector3D{sScene.boat.position.x + 0.7f,
                         sScene.boat.position.y + 1.0f,
                         sScene.boat.position.z});
  shaderUniform(sScene.shaderBoat, "uGreenLight.cutOff",
                cos(10.0f * M_PIf / 180));

  for (unsigned int i = 0; i < sScene.boat.partModel.size(); i++) {
    auto &model = sScene.boat.partModel[i];
    glBindVertexArray(model.mesh.vao);

    shaderUniform(sScene.shaderBoat, "uModel", sScene.boat.transformation);

    for (auto &material : model.material) {
      /* set material properties */
      shaderUniform(sScene.shaderBoat, "uMaterial.diffuse", material.diffuse);
      shaderUniform(sScene.shaderBoat, "uMaterial.ambient", material.ambient);
      shaderUniform(sScene.shaderBoat, "uMaterial.specular", material.specular);
      shaderUniform(sScene.shaderBoat, "uMaterial.shininess",
                    material.shininess);

      glDrawElements(
          GL_TRIANGLES, material.indexCount, GL_UNSIGNED_INT,
          (const void *)(material.indexOffset * sizeof(unsigned int)));
    }
  }

  /* render water */
  {
    glUseProgram(sScene.shaderWater.id);
    /*Light on the water*/

    /* frontlight 1 */
    shaderUniform(sScene.shaderWater, "uFrontLight1.color",
                  Vector3D{1.0f, 1.0f, 1.0f});
    shaderUniform(sScene.shaderWater, "uFrontLight1.direction",
                  normalize(lightDirectionRot1 *
                            Vector3D{sScene.boat.transformation[0][0],
                                     sScene.boat.transformation[0][1] - 0.6f,
                                     sScene.boat.transformation[0][2]}));
    shaderUniform(sScene.shaderWater, "uFrontLight1.position",
                  Vector3D{sScene.boat.position.x + 1.0f,
                           sScene.boat.position.y + 1.0f,
                           sScene.boat.position.z});
    shaderUniform(sScene.shaderWater, "uFrontLight1.cutOff",
                  cos(75.0f * M_PIf / 180));

    /* frontlight 2 */
    shaderUniform(sScene.shaderWater, "uFrontLight2.color",
                  Vector3D{1.0f, 1.0f, 1.0f});
    shaderUniform(sScene.shaderWater, "uFrontLight2.direction",
                  normalize(lightDirectionRot1 *
                            Vector3D{sScene.boat.transformation[0][0],
                                     sScene.boat.transformation[0][1] - 0.6f,
                                     sScene.boat.transformation[0][2]}));
    shaderUniform(sScene.shaderWater, "uFrontLight2.position",
                  Vector3D{sScene.boat.position.x - 1.0f,
                           sScene.boat.position.y + 1.0f,
                           sScene.boat.position.z});
    shaderUniform(sScene.shaderWater, "uFrontLight2.cutOff",
                  cos(75.0f * M_PIf / 180));

    /* red light on the left-hand side */
    shaderUniform(sScene.shaderWater, "uRedLight.color",
                  Vector3D{0.53f, 0.04f, 0.04f});
    shaderUniform(sScene.shaderWater, "uRedLight.direction",
                  lightDirectionRot3 *
                      Vector3D(sScene.boat.transformation[0][0],
                               sScene.boat.transformation[0][1] - 0.7f,
                               sScene.boat.transformation[0][2]));
    shaderUniform(sScene.shaderWater, "uRedLight.position",
                  Vector3D{sScene.boat.position.x + 0.7f,
                           sScene.boat.position.y + 1.0f,
                           sScene.boat.position.z + 0.0f});
    shaderUniform(sScene.shaderWater, "uRedLight.cutOff",
                  cos(10.0f * M_PIf / 180));

    /* green light on the right-hand side */
    shaderUniform(sScene.shaderWater, "uGreenLight.color",
                  Vector3D{0.0f, 0.39f, 0.0f});
    shaderUniform(sScene.shaderWater, "uGreenLight.direction",
                  Vector3D(sScene.boat.transformation[0][0],
                           sScene.boat.transformation[0][1] - 0.7f,
                           sScene.boat.transformation[0][2]));
    shaderUniform(sScene.shaderWater, "uGreenLight.position",
                  Vector3D{sScene.boat.position.x - 0.7f,
                           sScene.boat.position.y + 1.0f,
                           sScene.boat.position.z});
    shaderUniform(sScene.shaderWater, "uGreenLight.cutOff",
                  cos(10.0f * M_PIf / 180));
    /* setup camera and model matrices */
    shaderUniform(sScene.shaderWater, "uProj", proj);
    shaderUniform(sScene.shaderWater, "uView", view);
    shaderUniform(sScene.shaderWater, "uModel", Matrix4D::identity());
    shaderUniform(sScene.shaderWater, "uCamera.position",
                  sScene.camera.position);
    /* Set shader for light */
    shaderUniform(sScene.shaderWater, "uLight.ambientColor",
                  sScene.light.ambientColor);
    shaderUniform(sScene.shaderWater, "uLight.color", sScene.light.color);
    shaderUniform(sScene.shaderWater, "uLight.direction",
                  sScene.light.direction);
    shaderUniform(sScene.shaderWater, "uLight.ambientCoeff",
                  sScene.light.ambientCoeff);
    shaderUniform(sScene.shaderWater, "uLight.diffuseCoeff",
                  sScene.light.diffuseCoeff);
    shaderUniform(sScene.shaderWater, "uLight.specularCoeff",
                  sScene.light.specularCoeff);
    /* set material properties */
    shaderUniform(sScene.shaderWater, "uMaterial.diffuse",
                  sScene.water.material.front().diffuse);
    shaderUniform(sScene.shaderWater, "uMaterial.ambient",
                  sScene.water.material.front().ambient);
    shaderUniform(sScene.shaderWater, "uMaterial.specular",
                  sScene.water.material.front().specular);
    shaderUniform(sScene.shaderWater, "uMaterial.shininess",
                  sScene.water.material.front().shininess);

    /*setup waterSim parameter for water-vertex shader*/
    shaderUniform(sScene.shaderWater, "u_lastTime", sScene.waterSim.accumTime);
    shaderUniform(sScene.shaderWater, "u_direction_0",
                  sScene.waterSim.parameter[0].direction);
    shaderUniform(sScene.shaderWater, "u_direction_1",
                  sScene.waterSim.parameter[1].direction);
    shaderUniform(sScene.shaderWater, "u_direction_2",
                  sScene.waterSim.parameter[2].direction);

    glBindVertexArray(sScene.water.mesh.vao);
    glDrawElements(GL_TRIANGLES, sScene.water.material.front().indexCount,
                   GL_UNSIGNED_INT,
                   (const void *)(sScene.water.material.front().indexOffset *
                                  sizeof(unsigned int)));
  }

  /* cleanup opengl state */
  glBindVertexArray(0);
  glUseProgram(0);
}

void sceneDraw() {
  glClearColor(sScene.light.background.x, sScene.light.background.y,
               sScene.light.background.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*------------ render scene -------------*/
  render();

  /* cleanup opengl state */
  glBindVertexArray(0);
  glUseProgram(0);
}

int main(int argc, char **argv) {
  /*---------- init window ------------*/
  int width = 1280;
  int height = 720;
  GLFWwindow *window =
      windowCreate("Assignment 2 - Shader Programming", width, height);
  if (!window) {
    return EXIT_FAILURE;
  }

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
  while (!glfwWindowShouldClose(window)) {
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
