// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <imgui.h>

// src
#include "PearlEngine.h"
#include "Time.h"

// std
#include <cmath>
#include <iostream>
#include <memory>

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
  std::cout << "PearlEngine::~PearlEngine() -> Engine deconstructing"
            << std::endl;
  glfwTerminate();
}


// b@UPDATE
void PearlEngine::RunUpdateLoop() {
  GLFWwindow *window = pwin.GetWindow();

  // Create viewport framebuffer
  m_ViewportFramebuffer = std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  myCube.transform.Translate(glm::vec3(0.0f, 0.0f, -2.0f));

  // get the framebuffer size and set the aspect ratio
  int framebufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(pwin.GetWindow(), &framebufferWidth, &frameBufferHeight);
  float aspectRatio = (float)framebufferWidth / (float)frameBufferHeight;
  mainCamera.SetAspectRatio(aspectRatio);
  mainCamera.OutputParameters();

  // some openGL hints
  glFrontFace(GL_CW);
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);


  // glfw loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    Time::Update();
    ProcessInput(window);


    imGuiContext.BeginFrame();

    // === VIEWPORT ===
    ImGui::Begin("Viewport");

    // get available content region
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    // Resize framebuffer if needed
    if(viewportPanelSize.x > 0 && viewportPanelSize.y > 0){
      if(viewportPanelSize.x != m_ViewportSize.x || viewportPanelSize.y != m_ViewportSize.y){
        m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        m_ViewportFramebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
        mainCamera.SetAspectRatio(m_ViewportSize.x / m_ViewportSize.y);
      }
    }

    // RENDER TO FRAMEBUFFER
    m_ViewportFramebuffer->Bind();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render scene
    myShader.Use();
    float time = glfwGetTime();
    myCube.transform.Rotate(Time::deltaTime * 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
    myCube.transform.Rotate(Time::deltaTime * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    myShader.SetMatrix4(myCube.transform.GetModelMatrix(), "transform");
    myShader.SetMatrix4(mainCamera.GetProjectionMatrix(), "projection");
    myShader.SetMatrix4(mainCamera.GetViewMatrix(), "view");
    myCube.Render(myShader);

    m_ViewportFramebuffer->Unbind();

    // Display framebuffer texture in ImGui
    ImGui::Image(
      (void*)(intptr_t)m_ViewportFramebuffer->GetTextureID(),
      viewportPanelSize,
      ImVec2(0,1), ImVec2(1, 0) // Flip vertically (OpenGL coords)
    );

    ImGui::End();

    // === RENDER IMGUI TO DEFAULT RENDERER ===
    int displayW, displayH;
    glfwGetFramebufferSize(window, &displayW, &displayH);
    glViewport(0, 0, displayW, displayH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    imGuiContext.EndFrame();

    glfwSwapBuffers(window);
  }
}

void PearlEngine::ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}


