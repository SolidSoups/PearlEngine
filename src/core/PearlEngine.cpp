// libResourceSystem.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <gtk-4.0/gtk/gtk.h>
#include <imgui.h>

// src
#include "AssetEditorPanel.h"
#include "CameraComponent.h"
#include "InspectorEditorPanel.h"
#include "LoggerEditorPanel.h"
#include "MenuRegistry.h"
#include "PearlEngine.h"
#include "MessageBus.h"
#include "MessageQueue.h"
#include "Project.h"
#include "ProjectEditorPanel.h"
#include "Renderer.h"
#include "ResourceEditorPanel.h"
#include "SceneHierarchyEditorPanel.h"
#include "SelectionWizard.h"
#include "Time.h"
#include "TransformComponentEditor.h"
#include "ViewportEditorPanel.h"

#include "Mesh_ResourceConverter.h"
#include "Texture_ResourceConverter.h"
#include "Logger.h"
#include "MaterialData.h"
#include "ResourceSystem.h"
#include "TextureData.h"
#include "converters/OBJ_AssetConverter.h"
#include "converters/Texture_AssetConverter.h"

#include "materialLoaders.h"
#include "shaderLoaders.h"
#include "textureLoaders.h"

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
  ServiceLocator::Provide(&m_AssetSystem);
  ServiceLocator::Provide(&m_ResourceSystem);
  ServiceLocator::Provide(m_MessageBus.get());
  ServiceLocator::Provide(m_MessageQueue.get());

  // Register asset converters
  m_AssetSystem.AssetConverters.Register(
      "Mesh_Asset", std::make_unique<OBJ_AssetConverter>());
  m_AssetSystem.AssetConverters.Register(
      ".obj", std::make_unique<OBJ_AssetConverter>());

  m_AssetSystem.AssetConverters.Register(
      "Texture_Asset", std::make_unique<Texture_AssetConverter>());
  m_AssetSystem.AssetConverters.Register(
      ".png", std::make_unique<Texture_AssetConverter>());
  m_AssetSystem.AssetConverters.Register(
      ".jpg", std::make_unique<Texture_AssetConverter>());

  // Register resource converters
  m_ResourceSystem.AssetConverters.Register(
      std::type_index(typeid(Mesh_Asset)),
      std::make_unique<Mesh_ResourceConverter>());
  m_ResourceSystem.AssetConverters.Register(
      std::type_index(typeid(Texture_Asset)),
      std::make_unique<Texture_ResourceConverter>());

  isInitialized = true;

  LOG_INFO << "Engine constructed and initialized";
}

PearlEngine::~PearlEngine() {
  LOG_INFO << "Engine deconstructing";
  m_ResourceSystem.DestroyAllResources();
  ServiceLocator::Reset();
  glfwTerminate();
}

void PearlEngine::Initialize() {
  LOG_INFO << "Beginning initialization";
  GLFWwindow *window = pwin.GetWindow();
  glfwMakeContextCurrent(window);

  m_CameraController = std::make_unique<CameraController>(&m_Camera);

  // initialize the time
  Time::Initialize();

  // Load textures (using ResourceSystem)
  TextureHandle sunshineTextureHandle =
      LoadTexture(&m_ResourceSystem, "assets/sunshine.png");
  TextureHandle pearlTextureHandle =
      LoadTexture(&m_ResourceSystem, "assets/pearl.png");

  // Create shader (using ResourceSystem)
  m_ShaderHandle =
      CreateShader(&m_ResourceSystem, "shaders/vert.glsl", "shaders/frag.glsl");

  // Create new shader
  ShaderHandle shadHandle = CreateShader(
      &m_ResourceSystem, "shaders/vertNew.glsl", "shaders/fragNew.glsl");

  // Create new materials for pearl and sunshine
  MaterialHandle sunMatHandle =
      CreateMaterial(&m_ResourceSystem, m_ShaderHandle);
  MaterialSetTexture(&m_ResourceSystem, sunMatHandle, "mainTexture",
                     sunshineTextureHandle);
  MaterialHandle pearlMatHandle =
      CreateMaterial(&m_ResourceSystem, m_ShaderHandle);
  MaterialSetTexture(&m_ResourceSystem, pearlMatHandle, "mainTexture",
                     pearlTextureHandle);

  // Create new material
  // MaterialHandle newMat = CreateMaterial(&m_ResourceSystem, shadHandle);
  // MaterialSetTexture(&m_ResourceSystem, newMat, "mainTexture",
  //                    sunshineTextureHandle);

  // create the main camera
  GameObject *cameraGO = m_Scene.CreateGameObject("Main Camera");
  auto *cmCmp = cameraGO->AddComponent<CameraComponent>();
  cameraGO->AddComponent<TransformComponent>(cmCmp->cameraData.position);
  m_Scene.SetActiveCamera(cmCmp);

  // Create the weird mesh
  // const pe::FileDescriptor* houseFile =
  // pe::Project::Get().FindFile("medieval house", ".obj"); MeshHandle
  // houseMesh = CreateMeshFromObjFile(houseFile->localPath.c_str());
  // TextureHandle houseTex =
  // LoadTexture("assets/31-village-house/house2.png"); MaterialHandle
  // houseMat = CreateMaterial(shadHandle); MaterialSetTexture(houseMat,
  // "mainTexture", houseTex); auto* houseGameObject =
  // m_Scene.CreateGameObject();
  // houseGameObject->AddComponent<RenderComponent>(houseMesh, houseMat);
  // houseGameObject->AddComponent<TransformComponent>();

  // for(float x = -2.0f; x <= 2.0f; x += 2.0f){
  //   for(float y = -2.0f; y <= 2.0f; y += 2.0f){
  //     GameObject* go = m_Scene.CreateGameObject();
  //     go->AddComponent<RenderComponent>(cubeMeshOldHandle, sunMatHandle);
  //     go->AddComponent<TransformComponent>(glm::vec3(x, y, 0.0f));
  //   }
  // }

  // GameObject* weirdGo = m_Scene.CreateGameObject("loaded obj mesh");
  // weirdGo->AddComponent<RenderComponent>(newMesh, newMat);
  // weirdGo->AddComponent<TransformComponent>(glm::vec3(0.f, 0.f, 0.f));

  // Create viewport framebuffer
  m_ViewportFramebuffer =
      std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  // Create the viewport editor panel
  m_ViewportPanel =
      m_GUIContext.AddPanel<ViewportEditorPanel>(m_ViewportFramebuffer.get());
  m_GUIContext.AddPanel<SceneHierarchyEditorPanel>();
  m_GUIContext.AddPanel<ResourceEditorPanel>(m_ResourceSystem);
  m_GUIContext.AddPanel<InspectorEditorPanel>();
  m_GUIContext.AddPanel<LoggerEditorPanel>();
  m_GUIContext.AddPanel<ProjectEditorPanel>();
  m_GUIContext.AddPanel<AssetEditorPanel>(sunMatHandle);
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
  m_Scene.Render(&m_ResourceSystem, m_Camera);

  m_ViewportFramebuffer->Unbind();
}

void PearlEngine::RenderEditor() {
  m_GUIContext.BeginFrame();
  m_GUIContext.RenderEditorPanels();

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
  } else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
  }

  ImGuiIO io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;
}

void PearlEngine::AddMenuBarItems() {
  MenuRegistry::Get().Register("File/Exit", [this]() {
    glfwSetWindowShouldClose(pwin.GetWindow(), true);
  });
}
