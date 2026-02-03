#pragma once

#include "GUIContext.h"
#include "PearlWindow.h"
#include "GBuffer.h"
#include "Mesh.h"
#include "Camera.h"
#include "ShaderData.h"
#include "Scene.h"
#include "SelectionWizard.h"
#include "MessageBus.h"
#include "MessageQueue.h"

class Framebuffer;
class ViewportEditorPanel;
class CameraController;

class MeshManager;
class TextureManager;
class ShaderManager;

#include <memory>

struct GLFWwindow;

class PearlEngine {
public:
  PearlEngine();
  ~PearlEngine();

  void Initialize();
  void RunUpdateLoop();
  void Update();
  void Render();
  void RenderEditor();

  void GeometryRenderPass();
  void QuadDebugRenderPass();
  void LightingPass();
  void FlatShadePass();

  inline bool IsInitialized() { return isInitialized; }

private:
  void ProcessInput(GLFWwindow *window);

private:
  void AddMenuBarItems();

public:
  Scene m_Scene;
  Camera m_Camera{};
  std::unique_ptr<CameraController> m_CameraController;

private:
  std::unique_ptr<SelectionWizard> m_SelectionWizard;
  std::unique_ptr<MessageBus> m_MessageBus;
  std::unique_ptr<MessageQueue> m_MessageQueue;

  std::unique_ptr<MeshManager> m_MeshManager;
  std::unique_ptr<TextureManager> m_TextureManager;
  std::unique_ptr<ShaderManager> m_ShaderManager;

public:
  PearlWindow pwin{1800, 1000, "PearlEngine"};
  GUIContext m_GUIContext{pwin.GetWindow()};

  // framebuffer
  std::unique_ptr<Framebuffer> m_ViewportFramebuffer;
  glm::vec2 m_ViewportSize{1280, 70};
  ViewportEditorPanel *m_ViewportPanel = nullptr; // needed here

  // Gbuffer stuff
  std::unique_ptr<GBuffer> m_GBuffer;
  std::unique_ptr<Mesh> m_FullscreenQuad;

  std::shared_ptr<ShaderData> m_GeometryShader;
  std::shared_ptr<ShaderData> m_DisplayShader;
  std::shared_ptr<ShaderData> m_LightShader;
  std::shared_ptr<ShaderData> m_FlatShader;

private:
  bool isInitialized = false;
  bool bDebugGBuffer = false;
  bool bFlatShade = false;
};
