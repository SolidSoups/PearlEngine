// libResourceSystem.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <imgui.h>

// src
#include "PearlEngine.h"
#include "InspectorEditorPanel.h"
#include "LoggerEditorPanel.h"
#include "MenuRegistry.h"
#include "Renderer.h"
#include "Time.h"
#include "ViewportEditorPanel.h"
#include "Cube.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include "ResourceSystem.h"
#include "TextureData.h"
#include "MaterialData.h"
#include "Logger.h"

// std
#include <cmath>
#include <iostream>
#include <memory>

PearlEngine::PearlEngine() {
  if (!pwin.IsInitialized()) {
    isInitialized = false;
    LOG_INFO << "Engine failed to construct and initialize";
    return;
  }

  // store the engine as a user pointer in glfw
  glfwSetWindowUserPointer(pwin.GetWindow(), this);

  isInitialized = true;
  LOG_INFO << "Engine constructed and initialized";
}

PearlEngine::~PearlEngine() {
  LOG_INFO << "Engine deconstructing";
  glfwTerminate();
}

void PearlEngine::Initialize() {
  LOG_INFO << "Beginning initialization";
  GLFWwindow* window = pwin.GetWindow();
  glfwMakeContextCurrent(window);
  
  m_CameraController = std::make_unique<CameraController>(&m_Camera);

  // initialize the time
  LOG_INFO << "Initializing time";
  Time::Initialize();

  // Load textures (using ResourceSystem)
  LOG_INFO << "Creating textures";
  TextureHandle sunshineTextureHandle = LoadTexture("assets/sunshine.png");
  TextureHandle pearlTextureHandle = LoadTexture("assets/pearl.png");

  // Create shader (using ResourceSystem)
  LOG_INFO << "Creating shader";
  m_ShaderHandle = CreateShader("shaders/vert.glsl", "shaders/frag.glsl");

  // Create new materials for pearl and sunshine
  LOG_INFO << "Creating materials";
  MaterialHandle sunMatHandle = CreateMaterial(m_ShaderHandle);
  MaterialSetTexture(sunMatHandle, "mainTexture", sunshineTextureHandle);
  MaterialHandle pearlMatHandle = CreateMaterial(m_ShaderHandle);
  MaterialSetTexture(pearlMatHandle, "mainTexture", pearlTextureHandle);

  // Create the cube mesh
  LOG_INFO << "Creating cube mesh";
  MeshHandle cubeMeshHandle = CreateMesh(
    Cube::s_Vertices, 
    Cube::s_VertexCount, 
    Cube::s_Indices, 
    Cube::s_IndexCount);

  LOG_INFO << "Creating game objects";
  for(float x = -2.0f; x <= 2.0f; x += 2.0f){
    for(float y = -2.0f; y <= 2.0f; y += 2.0f){
      GameObject* go = m_Scene.CreateGameObject();
      go->AddComponent<RenderComponent>(cubeMeshHandle, sunMatHandle);
      go->AddComponent<TransformComponent>(glm::vec3(x, y, 0.0f));
    }
  }

  // Create viewport framebuffer
  LOG_INFO << "Creating viewport frame buffer";
  m_ViewportFramebuffer =
      std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  // Create the viewport editor panel
  LOG_INFO << "Creating editor panels";
  m_ViewportPanel = AddPanel<ViewportEditorPanel>(m_ViewportFramebuffer.get());
  m_ScenePanel = AddPanel<SceneHierarchyEditorPanel>(m_Scene, pearlMatHandle, sunMatHandle);
  m_ResourcePanel = AddPanel<ResourceEditorPanel>(ResourceSystem::Get());
  m_InspectorPanel = AddPanel<InspectorEditorPanel>(m_Scene);
  AddPanel<LoggerEditorPanel>();
  AddMenuBarItems();
   
  // Setup camera aspect ratio
  LOG_INFO << "Setting camera with correct aspect ratio";
  int framebufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(pwin.GetWindow(), &framebufferWidth,
                         &frameBufferHeight);
  float aspectRatio = (float)framebufferWidth / (float)frameBufferHeight;
  m_Camera.SetAspectRatio(aspectRatio);

  // OpenGL state configuration
  LOG_INFO << "Setting OpenGL state configurations";
  glFrontFace(GL_CW);
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  

  LOG_INFO << "Initialization finished";
}

// b@UPDATE
void PearlEngine::RunUpdateLoop() {
  GLFWwindow *window = pwin.GetWindow();

  Initialize();

  LOG_INFO << "Starting engine loop";
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

void PearlEngine::AddMenuBarItems(){
  MenuRegistry::Get().Register("File/Exit", [this](){
    glfwSetWindowShouldClose(pwin.GetWindow(), true);
  });
}
