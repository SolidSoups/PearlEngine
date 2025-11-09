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
#include "Material.h"
#include "Renderer.h"
#include "ViewportEditorPanel.h"

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

void PearlEngine::Initialize(){
  // initialize the time
  Time::Initialize();

  // Create shader
  m_Shader = std::make_unique<Shader>("shaders/vert.glsl", "shaders/frag.glsl");

  // Create cube
  m_Cube = std::make_unique<Cube>();
  m_Cube->transform.Translate(glm::vec3(0.0f, 0.0f, -2.0f));

  // Create material
  m_Material = std::make_unique<Material>(m_Shader.get());
  m_Cube->SetMaterial(m_Material.get());


  // Create viewport framebuffer
  m_ViewportFramebuffer = std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  // Create the viewport editor panel
  auto viewportPanel =
    std::make_unique<ViewportEditorPanel>(m_ViewportFramebuffer.get());
  m_ViewportPanel = viewportPanel.get();
  m_Panels.push_back(std::move(viewportPanel)); // Transfer ownership to vector

  // Setup camera aspect ratio
  int framebufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(pwin.GetWindow(), &framebufferWidth, &frameBufferHeight);
  float aspectRatio = (float)framebufferWidth / (float)frameBufferHeight;
  mainCamera.SetAspectRatio(aspectRatio);
  mainCamera.OutputParameters();

  // OpenGL state configuration
  glFrontFace(GL_CW);
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
}

// b@UPDATE
void PearlEngine::RunUpdateLoop() {
  GLFWwindow *window = pwin.GetWindow();

  Initialize();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ProcessInput(window);

    Time::Update();
    Update();

    Render();
    RenderEditor();

    glfwSwapBuffers(window);
  }
}

void PearlEngine::Update(){
    // handle viewport resize
    if(m_ViewportPanel->IsResized()){
      glm::vec2 newSize = m_ViewportPanel->GetSize();
      m_ViewportFramebuffer->Resize(newSize.x, newSize.y);
      mainCamera.SetAspectRatio(newSize.x / newSize.y);
    }

    // update objects
    m_Cube->transform.Rotate(Time::deltaTime * 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
    m_Cube->transform.Rotate(Time::deltaTime * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void PearlEngine::Render(){
  // Render scene to framebuffer
  m_ViewportFramebuffer->Bind();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Renderer::BeginScene(mainCamera);
  Renderer::Submit(*m_Cube.get());
  Renderer::EndScene();

  m_ViewportFramebuffer->Unbind();
}

void PearlEngine::RenderEditor(){
    imGuiContext.BeginFrame();

    // Render all editor panels 
    for(auto& panel : m_Panels){
      panel->OnImGuiRender();
    }

    // render imgui to the screen
    int displayW, displayH;
    glfwGetFramebufferSize(pwin.GetWindow(), &displayW, &displayH);
    glViewport(0, 0, displayW, displayH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    imGuiContext.Render();
}

void PearlEngine::ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}


