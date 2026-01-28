// std
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>

// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <gtk-4.0/gtk/gtk.h>
#include <imgui.h>

// src
#include "PearlEngine.h"
#include "AmbientLightEditorPanel.h"
#include "CameraController.h"
#include "Project.h"
#include "UserGUI.h"
#include "Renderer.h"
#include "Time.h"
#include "ServiceLocator.h"
#include "SelectionWizard.h"
#include "Defaults.h"
#include "MessageBus.h"
#include "MessageQueue.h"
#include "CameraComponent.h"
#include "Renderer.h"

#include "MeshManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "MaterialLoader.h"

// editor
#include "MenuRegistry.h"
#include "MemoryEditorPanel.h"
#include "InspectorEditorPanel.h"
#include "LoggerEditorPanel.h"
#include "SceneHierarchyEditorPanel.h"
#include "TransformComponentEditor.h"
#include "ViewportEditorPanel.h"
#include "FileSystemEditorPanel.h"

#include "Logger.h"
#include "Material.h"
#include "TextureData.h"

// #define DEBUG_GBUFFER

PearlEngine::PearlEngine() {
  if (!pwin.IsInitialized()) {
    isInitialized = false;
    return;
  }

  // Create owned services
  m_SelectionWizard = std::make_unique<SelectionWizard>();
  m_MessageBus = std::make_unique<MessageBus>();
  m_MessageQueue = std::make_unique<MessageQueue>();

  // Create resource managers
  m_MeshManager = std::make_unique<MeshManager>();
  m_TextureManager = std::make_unique<TextureManager>();
  m_ShaderManager = std::make_unique<ShaderManager>();

  // Register all services with static ServiceLocator
  ServiceLocator::Provide(&m_Scene);
  ServiceLocator::Provide(&m_Camera);
  ServiceLocator::Provide(m_SelectionWizard.get());
  ServiceLocator::Provide(&pwin);
  ServiceLocator::Provide(m_MessageBus.get());
  ServiceLocator::Provide(m_MessageQueue.get());
  ServiceLocator::Provide(m_MeshManager.get());
  ServiceLocator::Provide(m_TextureManager.get());
  ServiceLocator::Provide(m_ShaderManager.get());
  Defaults::Init();

  isInitialized = true;

}

PearlEngine::~PearlEngine() {
  ServiceLocator::Destroy();
  UserGUI::Destroy();
  glfwTerminate();
}

void PearlEngine::Initialize() {
  GLFWwindow *window = pwin.GetWindow();
  glfwMakeContextCurrent(window);

  m_CameraController = std::make_unique<CameraController>(&m_Camera);

  // initialize the time
  Time::Initialize();

  // Load textures using new loaders
  if (!ServiceLocator::IsReady<TextureManager>()) {
    LOG_ERROR << "TextureManager is not ready!";
  }
  auto sunshineTexture =
      ServiceLocator::Get<TextureManager>().load("assets/sunshine.png");
  auto pearlTexture =
      ServiceLocator::Get<TextureManager>().load("assets/pearl.png");

  // Create shaders using new loaders
  auto shader = Defaults::getDefaultShader();

  // create shaders
  m_GeometryShader = m_ShaderManager->load("shaders/geometryVert.glsl",
                                           "shaders/geometryFrag.glsl");
  m_DisplayShader = m_ShaderManager->load("shaders/displayVert.glsl",
                                          "shaders/displayFrag.glsl");
  m_LightShader =
      m_ShaderManager->load("shaders/lightVert.glsl", "shaders/lightFrag.glsl");

  // Create materials using new loaders
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
  cameraGO->GetComponent<TransformComponent>()->position = cmCmp->cameraData.position;
  m_Scene.SetActiveCamera(cmCmp);

  // create house model
  auto go1 = m_Scene.CreateGameObject("Test house");
  auto houseRenderComp = go1->AddComponent<RenderComponent>();
  houseRenderComp->mesh = m_MeshManager->loadOBJ("assets/viking house/Viking_House.obj");
  houseRenderComp->material = MaterialLoader::create(Defaults::getDefaultShader());
  auto houseTex = m_TextureManager->load("assets/viking house/Viking_House.png");
  houseRenderComp->material->setTexture("texture_diffuse1", houseTex);

  // create some lights in the scene
  for(int i=0; i<3; i++){
    auto newLight = m_Scene.CreatePointLight();
    newLight->GetComponent<TransformComponent>()->position 
      = glm::vec3(-6.f + i*4.f, 8.f, -4.5f + i*3.f);
    auto lightComp = newLight->GetComponent<PointLightComponent>();
    lightComp->data.radius = 8.f;
  }

  // Create viewport framebuffer
  m_ViewportFramebuffer =
      std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  // initialize g buffer
  m_GBuffer = std::make_unique<GBuffer>(m_ViewportSize.x, m_ViewportSize.y);
  Renderer::SetGeometryPassEnabled(true);

  // create the fullscreen quad
  std::vector<float> quadVertices = {
      // positions        // uv       // normals
      -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
      1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top-right
  };

  std::vector<unsigned int> quadIndices = {
      0, 1, 2, // first triangle
      0, 2, 3  // second triangle
  };
  m_FullscreenQuad = std::make_unique<Mesh>(quadVertices, quadIndices);

  // Create the viewport editor panel
  m_ViewportPanel =
      m_GUIContext.AddPanel<ViewportEditorPanel>(m_ViewportFramebuffer.get());
  m_GUIContext.AddPanel<SceneHierarchyEditorPanel>();
  m_GUIContext.AddPanel<InspectorEditorPanel>();
  m_GUIContext.AddPanel<LoggerEditorPanel>();
  m_GUIContext.AddPanel<FileSystemEditorPanel>();
  m_GUIContext.AddPanel<MemoryEditorPanel>();
  m_GUIContext.AddPanel<AmbientLightEditorPanel>();
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

    // process messages
    auto messages = ServiceLocator::Get<MessageQueue>().DrainAll();
    auto &msgBus = ServiceLocator::Get<MessageBus>();
    for (auto &msg : messages) {
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
    m_GBuffer->resize(newSize.x, newSize.y);

    m_ViewportSize.x = newSize.x;
    m_ViewportSize.y = newSize.y;
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
  GeometryRenderPass();
#ifdef DEBUG_GBUFFER
  QuadDebugRenderPass();
#else
  LightingPass();
#endif
}

void PearlEngine::LightingPass() {
  m_ViewportFramebuffer->Bind();
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetPositionTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetNormalTexture());
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetAlbedoSpecTexture());
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetDepthTexture());

  // send light uniforms
  m_LightShader->use();
  // we need to set the texture uniforms
  m_LightShader->setInt("gPosition", 0);
  m_LightShader->setInt("gNormal", 1);
  m_LightShader->setInt("gAlbedoSpec", 2);
  m_LightShader->setInt("gDepthStencil", 3);
  Renderer::SubmitLights(m_Scene.GetPointLights());
  Renderer::SendLightUniforms(m_LightShader);
  m_LightShader->setVec3("viewPos", m_Camera.GetCurrentTarget()->position);
  glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
  m_FullscreenQuad->Draw();
  m_ViewportFramebuffer->Unbind();
}

void PearlEngine::QuadDebugRenderPass() {
  m_ViewportFramebuffer->Bind();
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_DisplayShader->use();
  m_DisplayShader->setInt("textureSampler", 0);

  int halfWidth = m_ViewportSize.x / 2;
  int halfHeight = m_ViewportSize.y / 2;

  // top left quadrant
  glViewport(0, halfHeight, halfWidth, halfHeight);
  m_DisplayShader->setInt("bufferType", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetPositionTexture());
  m_FullscreenQuad->Draw();

  // top right quadrant
  glViewport(halfWidth, halfHeight, halfWidth, halfHeight);
  m_DisplayShader->setInt("bufferType", 1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetNormalTexture());
  m_FullscreenQuad->Draw();

  // bottom left quadrant
  glViewport(0, 0, halfWidth, halfHeight);
  m_DisplayShader->setInt("bufferType", 2);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetAlbedoSpecTexture());
  m_FullscreenQuad->Draw();

  // bottom rigth quadrant
  glViewport(halfWidth, 0, halfWidth, halfHeight);
  m_DisplayShader->setInt("bufferType", 3);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetDepthTexture());
  m_FullscreenQuad->Draw();

  // reset viewport
  glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

  m_ViewportFramebuffer->Unbind();
}

void PearlEngine::GeometryRenderPass() {
  m_GBuffer->bind();
  glClearColor(0, 0, 0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Renderer::SetGeometryPassEnabled(true);
  Renderer::SetNextShader(m_GeometryShader);
  m_GeometryShader->use();

  m_Scene.Render(m_Camera);

  Renderer::SetGeometryPassEnabled(false);
  m_GBuffer->unbind();
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
  } else if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
    m_ShaderManager->recompileAll();
  }

  ImGuiIO io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;
}

void PearlEngine::AddMenuBarItems() {
  MenuRegistry::Get().Register("File/Exit", [this]() {
    glfwSetWindowShouldClose(pwin.GetWindow(), true);
  });

  MenuRegistry::Get().Register("Tools/Reload Shaders", [this]() {
    m_ShaderManager->recompileAll();
  });
}
