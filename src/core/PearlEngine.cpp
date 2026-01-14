// libResourceSystem.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <gtk-4.0/gtk/gtk.h>
#include <imgui.h>

// src
#include "ServiceLocator.h"
#include "UserGUI.h"
#include "CameraComponent.h"
#include "Defaults.h"
#include "MemoryEditorPanel.h"
#include "MenuRegistry.h"
#include "PearlEngine.h"
#include "MessageBus.h"
#include "MessageQueue.h"
#include "Project.h"
#include "Renderer.h"
#include "SelectionWizard.h"
#include "Time.h"

#include "MeshManager.h"
#include "TextureManager.h"

// editor panels
#include "InspectorEditorPanel.h"
#include "LoggerEditorPanel.h"
#include "SceneHierarchyEditorPanel.h"
#include "TransformComponentEditor.h"
#include "ViewportEditorPanel.h"
#include "FileSystemEditorPanel.h"

#include "Logger.h"
#include "Material.h"
#include "TextureData.h"

#include "ShaderManager.h"
#include "TextureManager.h"
#include "MaterialLoader.h"

// std
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>

PearlEngine::PearlEngine() {
  if (!pwin.IsInitialized()) {
    isInitialized = false;
    LOG_INFO << "Engine failed to construct and initialize";
    return;
  }

  // Create owned services
  m_SelectionWizard = std::make_unique<SelectionWizard>();
  m_MessageBus = std::make_unique<MessageBus>();
  m_MessageQueue = std::make_unique<MessageQueue>();

  // Register all services with static ServiceLocator
  ServiceLocator::Provide(&m_Scene);
  ServiceLocator::Provide(&m_Camera);
  ServiceLocator::Provide(m_SelectionWizard.get());
  ServiceLocator::Provide(&pwin);
  ServiceLocator::Provide(m_MessageBus.get());
  ServiceLocator::Provide(m_MessageQueue.get());
  ServiceLocator::Provide(new MeshManager);
  ServiceLocator::Provide(new TextureManager);
  ServiceLocator::Provide(new ShaderManager);
  Defaults::Init();

  isInitialized = true;

  LOG_INFO << "Engine constructed and initialized";
}

PearlEngine::~PearlEngine() {
  LOG_INFO << "Engine deconstructing";
  ServiceLocator::Reset();
  UserGUI::Destroy();
  glfwTerminate();
}

void PearlEngine::Initialize() {
  LOG_INFO << "Beginning initialization";
  GLFWwindow *window = pwin.GetWindow();
  glfwMakeContextCurrent(window);

  m_CameraController = std::make_unique<CameraController>(&m_Camera);

  // initialize the time
  Time::Initialize();

  // Load textures using new loaders
  LOG_INFO << "loading textures";
  if(!ServiceLocator::IsReady<TextureManager>()){
    LOG_ERROR << "TextureManager is not ready!";
  }
  auto sunshineTexture = ServiceLocator::Get<TextureManager>().load("assets/sunshine.png");
  auto pearlTexture = ServiceLocator::Get<TextureManager>().load("assets/pearl.png");

  // Create shaders using new loaders
  LOG_INFO << "Setting default shader";
  auto shader = Defaults::getDefaultShader();

  // Create materials using new loaders
  LOG_INFO << "setting texture";
  MaterialLoader matLoader;
  auto sunMaterial = matLoader.create(shader);
  if (sunMaterial && sunshineTexture) {
      sunMaterial->setTexture("albedoMap", sunshineTexture);
  }

  auto pearlMaterial = matLoader.create(shader);
  if (pearlMaterial && pearlTexture) {
      pearlMaterial->setTexture("albedoMap", pearlTexture);
  }

  // create the main camera
  GameObject *cameraGO = m_Scene.CreateGameObject("Main Camera");
  auto *cmCmp = cameraGO->AddComponent<CameraComponent>();
  cameraGO->AddComponent<TransformComponent>(cmCmp->cameraData.position);
  m_Scene.SetActiveCamera(cmCmp);


  auto go1 = m_Scene.CreateGameObject();
  go1->AddComponent<TransformComponent>();
  go1->AddComponent<RenderComponent>();

  // Create viewport framebuffer
  m_ViewportFramebuffer =
      std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  // Create the viewport editor panel
  m_ViewportPanel =
      m_GUIContext.AddPanel<ViewportEditorPanel>(m_ViewportFramebuffer.get());
  m_GUIContext.AddPanel<SceneHierarchyEditorPanel>();
  m_GUIContext.AddPanel<InspectorEditorPanel>();
  m_GUIContext.AddPanel<LoggerEditorPanel>();
  m_GUIContext.AddPanel<FileSystemEditorPanel>();
  m_GUIContext.AddPanel<MemoryEditorPanel>();
  AddMenuBarItems();

  // Setup camera aspect ratio
  int framebufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(pwin.GetWindow(), &framebufferWidth,
                         &frameBufferHeight);
  float aspectRatio = (float)framebufferWidth / (float)frameBufferHeight;
  m_Camera.SetAspect(aspectRatio);

  // OpenGL state configuration
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

    // process messages
    auto messages = ServiceLocator::Get<MessageQueue>().DrainAll();
    if (messages.size() > 0)
      LOG_INFO << "Drained " << messages.size() << " messages!";
    auto &msgBus = ServiceLocator::Get<MessageBus>();
    for (auto &msg : messages) {
      LOG_INFO << "Processing and dispatching a message";
      msgBus.Dispatch(msg);
    }

    Render();
    RenderEditor();

    glfwSwapBuffers(window);
  }
}

void PearlEngine::Update() {
  // handle viewport resize
  if (m_ViewportPanel->IsResized()) {
    glm::vec2 newSize = m_ViewportPanel->GetSize();
    m_ViewportFramebuffer->Resize(newSize.x, newSize.y);
    m_Camera.SetAspect(newSize.x / newSize.y);
  }

  // Handle camera controls
  if (m_ViewportPanel->IsHovered()) {
    m_CameraController->OnUpdate(m_ViewportPanel->GetMouseDelta(),
                                 m_ViewportPanel->GetScrollDelta(),
                                 m_ViewportPanel->IsRightMouseDown(),
                                 m_ViewportPanel->IsMiddleMouseDown());
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
  m_GUIContext.BeginFrame();
  m_GUIContext.RenderEditorPanels();
  UserGUI::Render();

  // render imgui to the screen
  int displayW, displayH;
  glfwGetFramebufferSize(pwin.GetWindow(), &displayW, &displayH);
  glViewport(0, 0, displayW, displayH);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_GUIContext.Render();
}

void PearlEngine::ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    m_CameraController->Reset();
  } else if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS){
    LOG_INFO << "Recompiling all shaders!";
    ServiceLocator::Get<ShaderManager>().recompileAll();
  } 

  ImGuiIO io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;
}

void PearlEngine::AddMenuBarItems() {
  MenuRegistry::Get().Register("File/Exit", [this]() {
    glfwSetWindowShouldClose(pwin.GetWindow(), true);
  });

  MenuRegistry::Get().Register("Tools/Reload Shaders", []() {
    LOG_INFO << "Recompiling all shaders!";
    ServiceLocator::Get<ShaderManager>().recompileAll();
  });
}
