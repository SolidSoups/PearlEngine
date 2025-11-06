// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

// src
#include "PearlEngine.h"
#include "Time.h"

// std
#include <cmath>
#include <iostream>

PearlEngine::PearlEngine() {
  if (!pwin.IsInitialized()) {
    isInitialized = false;
    std::cout << "PearlEngine::PearlEngine() -> Engine failed to construct and "
                 "initialize"
              << std::endl;
    return;
  }

  // store the engine as a user pointer in glfw
  glfwSetWindowUserPointer(pwin.GetWindow(), this);

  // initialize the time
  Time::Initialize();

  isInitialized = true;
  std::cout
      << "PearlEngine::PearlEngine() -> Engine constructed and initialized"
      << std::endl;
}

PearlEngine::~PearlEngine() {
  // nothing to do here yet
  std::cout << "PearlEngine::~PearlEngine() -> Engine deconstructing"
            << std::endl;
}


// b@UPDATE
void PearlEngine::RunUpdateLoop() {
  GLFWwindow *window = pwin.GetWindow();

  glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 2.0f);
  glm::vec3 cube_pos = glm::vec3(0.0f, 0.0f, -2.0f); // unused

  // projection parameters
  const float aspect = (float)pwin.window_width / pwin.window_height;
  const float fov = glm::radians(60.0f);
  const float znear = 0.1f;
  const float zfar = 100.0f;

  // view matrix
  glm::mat4 view = glm::lookAt(cam_pos, glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));

  myCube.transform.Translate(glm::vec3(0.0f, 0.0f, -2.0f));

  // some openGL hints
  glFrontFace(GL_CW);
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);


  // glfw loop
  while (!glfwWindowShouldClose(window)) {
    Time::Update();
    ProcessInput(window);

    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    myShader.Use();

    // do transformation
    float time = glfwGetTime();
    myCube.transform.Rotate(Time::deltaTime * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
    myCube.transform.Rotate(Time::deltaTime * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    myShader.SetMatrix4(myCube.transform.GetModelMatrix(), "transform");


    // do projection
    auto projection = glm::perspective(fov, aspect, znear, zfar);
    myShader.SetMatrix4(mainCamera.GetProjectionMatrix(), "projection");

    // do view
    myShader.SetMatrix4(mainCamera.GetViewMatrix(), "view");

    myCube.Render(myShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void PearlEngine::ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}


