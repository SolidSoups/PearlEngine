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
#include "LineRenderer.h"
#include "SphereColliderComponent.h"
#include "RigidBodyComponent.h"
#include "PearlEngine.h"
#include "InputManager.h"
#include "AmbientLightEditorPanel.h"
#include "PointLightSystem.h"
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
#include "PointLightComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ecs_common.h"
#include "ScriptComponent.h"
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
  SelectionWizard::Init();
  m_MessageBus = std::make_unique<MessageBus>();
  m_MessageQueue = std::make_unique<MessageQueue>();
  m_InputManager = std::make_unique<InputManager>(pwin.GetWindow());

  // Create resource managers
  m_MeshManager = std::make_unique<MeshManager>();
  m_TextureManager = std::make_unique<TextureManager>();
  m_ShaderManager = std::make_unique<ShaderManager>();

  mEngineCamera = std::make_unique<EngineCamera>(*m_InputManager);

  mScene = std::make_unique<Scene>(mEngineCamera, m_InputManager);

  // Register all services with static ServiceLocator
  ServiceLocator::Provide<InputManager>(m_InputManager.get());
  ServiceLocator::Provide(mScene.get());
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
  LineRenderer::Destroy();
  ServiceLocator::Destroy();
  UserGUI::Destroy();

  // causes issues
  m_FullscreenQuad.reset();
}

void PearlEngine::Initialize() {
  LOG_INFO << "Initializing!";
  GLFWwindow *window = pwin.GetWindow();
  glfwMakeContextCurrent(window);

  // initialize the time
  Time::Initialize();

  // Load textures using new loaders
  if (!ServiceLocator::IsReady<TextureManager>()) {
    LOG_ERROR << "TextureManager is not ready!";
  }

  // Create shaders using new loaders
  auto shader = Defaults::getDefaultShader();

  // create shaders
  m_GeometryShader = m_ShaderManager->load("shaders/geometryVert.glsl",
                                           "shaders/geometryFrag.glsl");
  m_DisplayShader = m_ShaderManager->load("shaders/displayVert.glsl",
                                          "shaders/displayFrag.glsl");
  m_LightShader =
      m_ShaderManager->load("shaders/lightVert.glsl", "shaders/lightFrag.glsl");
  m_FlatShader =
      m_ShaderManager->load("shaders/flatVert.glsl", "shaders/flatFrag.glsl");
  m_GridShader =
      m_ShaderManager->load("shaders/gridVert.glsl", "shaders/gridFrag.glsl");

  auto lineShader =
      m_ShaderManager->load("shaders/lineVert.glsl", "shaders/lineFrag.glsl");
  LineRenderer::Initialize(lineShader);

  // create the main camera
  ecs::Entity cameraEntity = mScene->CreateEntity("Main Camera");
  auto &coordinator = mScene->GetCoordinator();
  coordinator.AddComponent(cameraEntity, CameraComponent{});
  auto &camComp = coordinator.GetComponent<CameraComponent>(cameraEntity);
  coordinator.GetComponent<TransformComponent>(cameraEntity).position =
      camComp.cameraData.position;
  mScene->SetActiveCamera(cameraEntity);

  // Create viewport framebuffer
  m_ViewportFramebuffer =
      std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  // initialize g buffer
  m_GBuffer = std::make_unique<GBuffer>(m_ViewportSize.x, m_ViewportSize.y);
  Renderer::SetGeometryPassEnabled(true);

  // create the fullscreen quad
  std::vector<float> quadVertices = {
      // positions         // uv         // normals
      -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, -1.0f, -1.0f, 0.0f,
      0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
      0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f,  1.0f,
  };
  std::vector<unsigned int> quadIndices = {
      0, 1, 2, // first triangle
      0, 2, 3  // second triangle
  };
  m_FullscreenQuad = std::make_unique<Mesh>(quadVertices, quadIndices);

  float plane_h = 1.0f * 1000;
  std::vector<float> planeQuadVerts = {
      -plane_h, 0, -plane_h, 0, 0, 0.0f, 1.0f, 0.0f, // bottom-left
      plane_h,  0, -plane_h, 1, 0, 0.0f, 1.0f, 0.0f, // bottom-left
      -plane_h, 0, plane_h,  0, 1, 0.0f, 1.0f, 0.0f, // bottom-left
      plane_h,  0, plane_h,  1, 1, 0.0f, 1.0f, 0.0f  // bottom-left
  };
  std::vector<unsigned int> planeQuadIndices = {0, 1, 2, 2, 1, 3};
  m_WorldPlaneQuad = std::make_unique<Mesh>(planeQuadVerts, planeQuadIndices);

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
  mScene->SetAspectRatio(aspectRatio);

  // OpenGL state configuration
  glFrontFace(GL_CW);
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  LOG_INFO << "Finished initialization";

  // create sphere comp
  auto &coord = mScene->GetCoordinator();
  auto sphere = mScene->CreateSphere("Script Tester");
  coord.GetComponent<TransformComponent>(sphere).position = glm::vec3{0, 5, 0};
  auto &renderC1 = coord.GetComponent<RenderComponent>(sphere);
  renderC1.material->setTexture(
      "texture_diffuse1",
      m_TextureManager->load("assets/Textures/Globe._Albedo.png"));
  coord.AddComponent(sphere, SphereColliderComponent{});
  coord.AddComponent(sphere, RigidBodyComponent{});

  // create the plane comp
  auto plane = mScene->CreatePlane("Collision test");
  coord.GetComponent<TransformComponent>(plane).scale = {10.f, 1.f, 10.f};
  auto &planeRender = coord.GetComponent<RenderComponent>(plane);
  planeRender.material->setTexture(
      "texture_diffuse1",
      m_TextureManager->load("assets/hatchet/Hatchet_diffuse.png"));
  coord.AddComponent(
      plane, BoxColliderComponent{{0.0f, -0.05f, 0.0f}, {10.f, 0.1f, 10.f}});
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
    // get new size
    glm::vec2 newSize = m_ViewportPanel->GetSize();
    float aspect = newSize.x / newSize.y;

    // resize core systems
    m_ViewportFramebuffer->Resize(newSize.x, newSize.y);
    m_GBuffer->resize(newSize.x, newSize.y);
    mScene->SetAspectRatio(aspect);

    // cache size
    m_ViewportSize.x = newSize.x;
    m_ViewportSize.y = newSize.y;
  }

  // Handle camera controls
  if (m_ViewportPanel->IsHovered()) {
    bool hasMoved = mEngineCamera->MoveCamera();

    // have we moved?
    auto *camSystem = mScene->GetCameraSystem();
    if (hasMoved &&
        camSystem->GetCameraMode() == CameraSystem::CameraMode::PREVIEW) {
      // set camera mode to ENGINE
      mScene->GetCameraSystem()->SetCameraMode(
          CameraSystem::CameraMode::ENGINE);

      // copy the preview camera view for seamless transition to engine cam
      auto previewCamEntity = mScene->GetCameraSystem()->GetPreviewEntity();
      if (previewCamEntity != ecs::NULL_ENTITY) {
        auto &coord = mScene->GetCoordinator();
        mEngineCamera->CopyEntity(
            coord.GetComponent<TransformComponent>(previewCamEntity),
            coord.GetComponent<CameraComponent>(previewCamEntity));
      }
    }
  }

  // update objects (currently does nothing, but ready for future!  )
  mScene->Update();
}

void PearlEngine::Render() {
  GeometryRenderPass();

  if (bDebugGBuffer) {
    QuadDebugRenderPass();
    return;
  }

  if (bFlatShade) {
    FlatShadePass();
  } else {
    LightingPass();
  }

  // Draw grid
  auto *camSystem = mScene->GetCameraSystem();
  glm::mat4 view, projection;
  camSystem->GetMatrices(view, projection);

  m_ViewportFramebuffer->Bind();
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_GridShader->use();
  m_GridShader->setMatrix4("view", view);
  m_GridShader->setMatrix4("projection", projection);
  m_GridShader->setVec3("cameraPos", camSystem->GetPosition());
  m_WorldPlaneQuad->Draw();
  glDisable(GL_BLEND);
  m_ViewportFramebuffer->Unbind();

  // draw gizmo lines
  m_ViewportFramebuffer->Bind();
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1.2f);
  LineRenderer::Flush(projection * view);
  glLineWidth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LINE_SMOOTH);
  m_ViewportFramebuffer->Unbind();
}

void PearlEngine::GeometryRenderPass() {
  m_GBuffer->bind();
  glClearColor(0.4, 0.4, 0.4, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Renderer::SetGeometryPassEnabled(true);
  Renderer::SetNextShader(m_GeometryShader);
  m_GeometryShader->use();

  mScene->Render(CameraSystem::CameraMode::ENGINE);

  Renderer::SetGeometryPassEnabled(false);
  m_GBuffer->unbind();
}

void PearlEngine::FlatShadePass() {
  m_ViewportFramebuffer->Bind();
  glClearColor(0.1f, 0.1f, 0.1f, 2.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetAlbedoSpecTexture());

  m_FlatShader->use();
  glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
  m_FullscreenQuad->Draw();
  m_ViewportFramebuffer->Unbind();
}

void PearlEngine::LightingPass() {
  auto camSystem = mScene->GetCameraSystem();

  m_ViewportFramebuffer->Bind();
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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

  // send light uniforms
  mScene->mPointLightSystem->SendUniforms(m_LightShader);
  m_LightShader->setFloat("ambientIntensity", mScene->ambientLight.intensity);
  m_LightShader->setVec4("ambientColor", mScene->ambientLight.color);
  m_LightShader->setVec3("viewPos", camSystem->GetPosition());

  glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
  m_FullscreenQuad->Draw();

  // Blit GBuffer depth into viewport framebuffer so grid depth-tests against
  // scene
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->GetFBOId());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ViewportFramebuffer->GetFBOId());
  glBlitFramebuffer(0, 0, m_ViewportSize.x, m_ViewportSize.y, 0, 0,
                    m_ViewportSize.x, m_ViewportSize.y, GL_DEPTH_BUFFER_BIT,
                    GL_NEAREST);

  // Re-bind viewport framebuffer as active draw target (without clearing)
  glBindFramebuffer(GL_FRAMEBUFFER, m_ViewportFramebuffer->GetFBOId());
  glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
}

void PearlEngine::QuadDebugRenderPass() {
  m_ViewportFramebuffer->Bind();
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_DisplayShader->use();
  m_DisplayShader->setInt("textureSampler", 0);

  // default: show all 4 viewports
  if (mDebugBufferIndex == 0) {
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
  } else {
    // Show single buffer framebuffer
    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
    glActiveTexture(GL_TEXTURE0);

    switch (mDebugBufferIndex) {
    case 1:
      m_DisplayShader->setInt("bufferType", 0);
      glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetPositionTexture());
      break;
    case 2:
      m_DisplayShader->setInt("bufferType", 1);
      glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetNormalTexture());
      break;
    case 3:
      m_DisplayShader->setInt("bufferType", 2);
      glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetAlbedoSpecTexture());
      break;
    case 4:
      m_DisplayShader->setInt("bufferType", 3);
      glBindTexture(GL_TEXTURE_2D, m_GBuffer->GetDepthTexture());
      break;
    }

    m_FullscreenQuad->Draw();
  }

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
  static bool gWasPressed = false;
  static bool f5WasPressed = false;

  m_InputManager->Update();

  // quit engine
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // reset camera controller
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    mEngineCamera->Reset();
  }

  // recompile all shaders
  if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS && !f5WasPressed) {
    m_ShaderManager->recompileAll();
    f5WasPressed = true;
  } else if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_RELEASE) {
    f5WasPressed = false;
  }

  if (bDebugGBuffer && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS &&
      !gWasPressed) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      mDebugBufferIndex = (mDebugBufferIndex + 4) % 5;
    } else {
      mDebugBufferIndex = (mDebugBufferIndex + 1) % 5;
    }
    gWasPressed = true;
  } else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) {
    gWasPressed = false;
  }

  static bool cSWasPressed = false;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
      glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
      !cSWasPressed) {
    // mScene->SaveScene();
    cSWasPressed = true;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
    cSWasPressed = false;
  }

  ImGuiIO io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;
}

void PearlEngine::AddMenuBarItems() {
  MenuRegistry::Get().Register("File/Exit", [this]() {
    glfwSetWindowShouldClose(pwin.GetWindow(), true);
  });
  MenuRegistry::Get().Register("File/Save scene as...", [this]() {
    // open a dialog to pick a name
    UserGUI::StartInputPopup(
        "Save scene as...", [this](const std::string &name) {
          const std::string assets_folder = "assets/";
          std::string filepath = assets_folder + name + ".json";
          mScene->SaveScene(filepath.c_str());
        });
  });
  MenuRegistry::Get().Register("File/Load scene...", [this]() {
    UserGUI::StartFilePopup(
        [this](const std::string &file) { mScene->LoadScene(file.c_str()); },
        {".json"});
  });

  MenuRegistry::Get().Register("Tools/Reload Shaders", [this]() {
    m_ShaderManager->recompileAll();
    LOG_INFO << "Recompiled all shaders";
  });

  MenuRegistry::Get().Register("Tools/Debug G-Buffer", &bDebugGBuffer);
  MenuRegistry::Get().Register("Tools/Flat Shade", &bFlatShade);
}
