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
#include <imgui.h>

// src
#include "CreateMeshMessageHandler.h"
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
#include "TerrainComponent.h"
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

  mScene->SetMeshManager(m_MeshManager.get());
  mScene->PostInitialization(&m_ViewportSize, &m_ViewportPos);

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

  auto playTexture = m_TextureManager->load("assets/ui/play_button.png");
  auto pauseTexture = m_TextureManager->load("assets/ui/pause_button.png");
  auto reloadTexture = m_TextureManager->load("assets/ui/reload_button.png");
  auto stopTexture = m_TextureManager->load("assets/ui/stop_button.png");
  m_GUIContext.SetToolbarCallbacks(
      [this]() {
        mRuntimeState = RUNTIME;
        mScene->OnRuntimeStart();
      },
      [this]() { mRuntimeState = mRuntimeState != PAUSED ? PAUSED : RUNTIME; },
      [this]() {
        mRuntimeState = EDITOR;
        mScene->OnRuntimeStop();
      },
      [this]() { mScene->OnSceneReload(); }, playTexture, pauseTexture,
      stopTexture, reloadTexture);

  // Create shaders using new loaders
  auto shader = Defaults::getDefaultShader();

  // create shaders
  mPickShader =
      m_ShaderManager->load("shaders/pickVert.glsl", "shaders/pickFrag.glsl");
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

  // // create the main camera
  // ecs::Entity cameraEntity = mScene->CreateEntity("Main Camera");
  // auto &coordinator = mScene->GetCoordinator();
  // coordinator.AddComponent(cameraEntity, CameraComponent{});
  // coordinator.GetComponent<TransformComponent>(cameraEntity).position =
  //     glm::vec3(0.0f, 0.0f, 5.0f);
  // mScene->SetActiveCamera(cameraEntity);

  // Create viewport framebuffer
  m_ViewportFramebuffer =
      std::make_unique<Framebuffer>(m_ViewportSize.x, m_ViewportSize.y);

  // create the picking framebuffer
  mPickFramebuf =
      std::make_unique<PickingFramebuffer>(m_ViewportSize.x, m_ViewportSize.y);

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
  glDisable(GL_CULL_FACE); // THIS IS SO WRONG
  glEnable(GL_DEPTH_TEST);

  // enable mesh loading through message bus
  IMessageHandler* meshHandler = new CreateMeshMessageHandler(); 
  m_MessageBus->Subscribe<CreateMeshMessage>(meshHandler);

  LOG_INFO << "Finished initialization";
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
  m_ViewportPos = m_ViewportPanel->GetViewportPos();

  // handle viewport resize
  if (m_ViewportPanel->IsResized()) {
    // get new size
    glm::vec2 newSize = m_ViewportPanel->GetSize();
    float aspect = newSize.x / newSize.y;

    // resize core systems
    m_ViewportFramebuffer->Resize(newSize.x, newSize.y);
    mPickFramebuf->Resize(newSize.x, newSize.y);
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

  // run scene if the game is running
  if (mRuntimeState == RUNTIME)
    mScene->Update();

  if (mScene->HasPendingLoad()) {
    LOG_INFO << "Scene has pending load";
    std::string path = mScene->ConsumePendingLoad();
    mScene->LoadScene(path.c_str());
    if (mRuntimeState == EDITOR) {
      m_CurrentScenePath = path;
      pwin.SetSceneTitle(path);
    }
  }
}

void PearlEngine::Render() {
  if (bDrawWireFrameMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  GeometryRenderPass();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if (bDebugGBuffer) {
    QuadDebugRenderPass();
    return;
  }

  if (bFlatShade) {
    FlatShadePass();
  } else {
    LightingPass();
  }

  auto *camSystem = mScene->GetCameraSystem();
  glm::mat4 view, projection;
  camSystem->GetMatrices(view, projection);

  // Draw grid
  if (bDrawGrid) {
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
  }

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

  // set selection through viewport
  if (m_ViewportPanel->DidLeftMouseClick() and mRuntimeState == EDITOR) {
    glm::vec2 clickPos = m_ViewportPanel->ConsumeLeftMouseClick();
    int px = (int)clickPos.x;
    int py = (int)clickPos.y;
    if (px < 0 or py < 0 or px >= (int)m_ViewportSize.x or
        py >= (int)m_ViewportSize.y)
      return; // out of bounds

    PickingRenderPass();
    ecs::Entity clickedEntity = (ecs::Entity)ReadPickedVal(px, py);
    if (clickedEntity != ecs::NULL_ENTITY) {
      SelectionWizard::Set(clickedEntity);
    } else
      SelectionWizard::Clear();
  }

  // Draw UI on top of everything
  m_ViewportFramebuffer->Bind();
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  mScene->RenderUI();
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  m_ViewportFramebuffer->Unbind();
}

void PearlEngine::PickingRenderPass() {
  mPickFramebuf->Bind();
  GLuint clearVal = 0xFFFFFFFF; // ecs::Entity = -1, which wraps to max val
  glClearBufferuiv(GL_COLOR, 0, &clearVal);
  glClear(GL_DEPTH_BUFFER_BIT);

  mPickShader->use();

  // upload camera matrices
  glm::mat4 view, proj;
  mScene->GetCameraSystem()->GetMatrices(view, proj);
  mPickShader->setMatrix4("view", view);
  mPickShader->setMatrix4("projection", proj);

  // upload all entities
  auto &entities = mScene->GetRenderEntities();
  auto &coord = mScene->GetCoordinator();
  for (ecs::Entity entity : entities) {
    auto &tf = coord.GetComponent<TransformComponent>(entity);
    auto &render = coord.GetComponent<RenderComponent>(entity);

    mPickShader->setMatrix4("transform", tf.GetModelMatrix());
    mPickShader->setUInt("entityID", entity);
    render.mesh->Draw();
  }

  mPickFramebuf->Unbind();
}
uint32_t PearlEngine::ReadPickedVal(unsigned int x, unsigned int y) {
  unsigned int flippedY = (unsigned int)m_ViewportSize.y - y;

  mPickFramebuf->Bind();
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  uint32_t id = ecs::NULL_ENTITY;
  glReadPixels(x, flippedY, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);
  mPickFramebuf->Unbind();

  return id;
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
  m_GUIContext.BeginFrame(mRuntimeState);
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
  // block all engine keybinds when imgui focuses
  ImGuiIO io = ImGui::GetIO();

  auto &input = m_InputManager;
  input->Update();

  // always on shortcuts
  if (input->GetKeyDown(GLFW_KEY_HOME) && input->GetKey(GLFW_KEY_LEFT_SHIFT))
    glfwSetWindowShouldClose(window, true);

  if (input->GetKeyDown(GLFW_KEY_F5))
    m_ShaderManager->recompileAll();

  // blocked when imgui has a text field focused
  if (!io.WantCaptureKeyboard) {
    if (SelectionWizard::HasSelection() && input->GetKeyDown(GLFW_KEY_D) &&
        input->GetKey(GLFW_KEY_LEFT_CONTROL)) {
      // duplicate entity
      auto newEntity = mScene->DuplicateEntity(SelectionWizard::Get());
      // let's not select the new entity, and instead keep the current entity
      // selected SelectionWizard::Set(newEntity);
    }

    // destroy entity with delete key
    if(SelectionWizard::HasSelection() && input->GetKeyDown(GLFW_KEY_DELETE) 
    && mRuntimeState == EDITOR){
      ecs::Entity selectedEntity = SelectionWizard::Get();
      mScene->DestroyEntity(selectedEntity);
    }

    if (input->GetKeyDown(GLFW_KEY_F)) {
      mEngineCamera->Reset();
    }

    if (bDebugGBuffer) {
      if (input->GetKeyDown(GLFW_KEY_G)) {
        if (input->GetKey(GLFW_KEY_LEFT_SHIFT))
          mDebugBufferIndex = (mDebugBufferIndex + 4) % 5;
        else
          mDebugBufferIndex = (mDebugBufferIndex + 1) % 5;
      }
    }

    // open scen
    if (input->GetKeyDown(GLFW_KEY_O)) {
      if (input->GetKey(GLFW_KEY_LEFT_CONTROL)) {
        UserGUI::StartFilePopup(
            [this](const std::string &file) {
              // load da scene
              mScene->RequestLoadScene(file.c_str());
            },
            {".json"});
      }
    }

    // toggle wireframe mode
    if (input->GetKeyDown(GLFW_KEY_B)) {
      bDrawWireFrameMode = !bDrawWireFrameMode;
    }

    if(input->GetKeyDown(GLFW_KEY_G)){
      bDrawGrid = !bDrawGrid;
    }

    // save and save as scene
    if (input->GetKeyDown(GLFW_KEY_S)) {
      if (input->GetKey(GLFW_KEY_LEFT_CONTROL) and
          input->GetKey(GLFW_KEY_LEFT_SHIFT)) {
        // save scene as
        UserGUI::StartInputPopup(
            "Save scene as...", [this](const std::string &name) {
              const std::string assets_folder = "assets/";
              std::string filepath = assets_folder + name + ".json";
              mScene->SaveScene(filepath.c_str());
              m_CurrentScenePath = filepath;
              pwin.SetSceneTitle(filepath);
            });
      } else if (input->GetKey(GLFW_KEY_LEFT_CONTROL)) {
        if (!m_CurrentScenePath.empty())
          mScene->SaveScene(m_CurrentScenePath.c_str());
      }
    }
  }
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
          m_CurrentScenePath = filepath;
          pwin.SetSceneTitle(filepath);
        });
  });
  MenuRegistry::Get().Register("File/Open scene...", [this]() {
    UserGUI::StartFilePopup(
        [this](const std::string &file) {
          // load da scene
          mScene->RequestLoadScene(file.c_str());
        },
        {".json"});
  });

  MenuRegistry::Get().Register("Tools/Reload Shaders", [this]() {
    m_ShaderManager->recompileAll();
    LOG_INFO << "Recompiled all shaders";
  });

  MenuRegistry::Get().Register("Tools/Debug G-Buffer", &bDebugGBuffer);
  MenuRegistry::Get().Register("Tools/Flat Shade", &bFlatShade);
}
