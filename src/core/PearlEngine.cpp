// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <imgui.h>

// src
#include "PearlEngine.h"
#include "Renderer.h"
#include "Time.h"
#include "ViewportEditorPanel.h"
#include "Cube.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include "ResourceSystem.h"
#include "ResourceManager.h"
#include "TextureData.h"
#include "MaterialData.h"

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

void PearlEngine::Initialize() {
  std::cout << "PearlEngine::Initialize() -> Beginning initialization" << std::endl;
  GLFWwindow* window = pwin.GetWindow();
  glfwMakeContextCurrent(window);
  
  m_CameraController = std::make_unique<CameraController>(&m_Camera);

  // initialize the time
  Time::Initialize();

  // Load textures (using ResourceSystem)
  TextureHandle sunshineTextureHandle = LoadTexture("assets/sunshine.png");
  TextureHandle pearlTextureHandle = LoadTexture("assets/pearl.png");

  // Create shader (using ResourceSystem)
  m_ShaderHandle = CreateShader("shaders/vert.glsl", "shaders/frag.glsl");

  // Create new materials for pearl and sunshine
  MaterialHandle sunMatHandle = CreateMaterial(m_ShaderHandle);
  MaterialSetTexture(sunMatHandle, "mainTexture", sunshineTextureHandle);
  MaterialHandle pearlMatHandle = CreateMaterial(m_ShaderHandle);
  MaterialSetTexture(pearlMatHandle, "mainTexture", pearlTextureHandle);

  // Create the cube mesh
  MeshHandle cubeMeshHandle = CreateMesh(
    Cube::s_Vertices, 
    Cube::s_VertexCount, 
    Cube::s_Indices, 
    Cube::s_IndexCount);

  std::cout << "Creating game objects" << std::endl;
  // Create some gameobjects
  GameObject* go1 = m_Scene.CreateGameObject();
  go1->AddComponent<TransformComponent>();
  go1->AddComponent<RenderComponent>(cubeMeshHandle, pearlMatHandle);

  GameObject* go2 = m_Scene.CreateGameObject();
  go2->AddComponent<TransformComponent>(glm::vec3(-2.0f, 0.0f, 0.0f));
  go2->AddComponent<RenderComponent>(cubeMeshHandle, sunMatHandle);

  GameObject* go3 = m_Scene.CreateGameObject();
  go3->AddComponent<TransformComponent>(glm::vec3(2.0f, 0.0f, 0.0f));
  go3->AddComponent<RenderComponent>(cubeMeshHandle, sunMatHandle);

  // Create viewport framebuffer
  m_ViewportFramebuffer =
      std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  // Create the viewport editor panel
  auto viewportPanel =
      std::make_unique<ViewportEditorPanel>(m_ViewportFramebuffer.get());
  m_ViewportPanel = viewportPanel.get();
  m_Panels.push_back(std::move(viewportPanel)); // Transfer ownership to vector

  // Setup camera aspect ratio
  int framebufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(pwin.GetWindow(), &framebufferWidth,
                         &frameBufferHeight);
  float aspectRatio = (float)framebufferWidth / (float)frameBufferHeight;
  m_Camera.SetAspectRatio(aspectRatio);
  m_Camera.OutputParameters();

  // OpenGL state configuration
  glFrontFace(GL_CW);
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  std::cout << "PearlEngine::Initialize() -> initialization finished" << std::endl;
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
  ResourceSystem::Get().Destroy();
}

void PearlEngine::Update() {
  // handle viewport resize
  if (m_ViewportPanel->IsResized()) {
    glm::vec2 newSize = m_ViewportPanel->GetSize();
    m_ViewportFramebuffer->Resize(newSize.x, newSize.y);
    m_Camera.SetAspectRatio(newSize.x / newSize.y);
  }

  // Handle camera controls
  if(m_ViewportPanel->IsHovered()){
    m_CameraController->OnUpdate(
      m_ViewportPanel->GetMouseDelta(),
      m_ViewportPanel->GetScrollDelta(),
      m_ViewportPanel->IsRightMouseDown(),
      m_ViewportPanel->IsMiddleMouseDown()
    );
  }

  // update objects (currently does nothing, but ready for future!  )
  m_Scene.Update();
}

void PearlEngine::Render() {
  // Render scene to framebuffer
  m_ViewportFramebuffer->Bind();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Scene handles all the rendering now!
  m_Scene.Render(m_Camera);

  m_ViewportFramebuffer->Unbind();
}

void PearlEngine::RenderEditor() {
  imGuiContext.BeginFrame();

  // Render all editor panels
  for (auto &panel : m_Panels) {
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
  else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    m_CameraController->Reset();
  }
}
