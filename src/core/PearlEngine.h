#pragma once

#include <memory>
#include "RuntimeState.h"
#include "PickingFramebuffer.h"
#include "GUIContext.h"
#include "PearlWindow.h"
#include "GBuffer.h"
#include "Mesh.h"
#include "Camera.h"
#include "ShaderData.h"
#include "Scene.h"
#include "MessageBus.h"
#include "MessageQueue.h"

class Framebuffer;
class ViewportEditorPanel;
class CameraController;
class InputManager;

class MeshManager;
class TextureManager;
class ShaderManager;


struct GLFWwindow;

class PearlEngine {
private:
  RuntimeState mRuntimeState = EDITOR;
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

  // specialized render passes
  void PickingRenderPass();
  uint32_t ReadPickedVal(unsigned int x, unsigned int y);

  inline bool IsInitialized() { return isInitialized; }

private:
  void ProcessInput(GLFWwindow *window);

private:
  void AddMenuBarItems();

private:
  std::unique_ptr<Scene> mScene;
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
  glm::vec2 m_ViewportPos{0, 0};
  ViewportEditorPanel *m_ViewportPanel = nullptr; // needed here
  std::unique_ptr<PickingFramebuffer> mPickFramebuf;

  // Gbuffer stuff
  std::unique_ptr<GBuffer> m_GBuffer;
  std::unique_ptr<Mesh> m_FullscreenQuad;
  std::unique_ptr<Mesh> m_WorldPlaneQuad;

  std::shared_ptr<ShaderData> m_GeometryShader;
  std::shared_ptr<ShaderData> m_DisplayShader;
  std::shared_ptr<ShaderData> m_LightShader;
  std::shared_ptr<ShaderData> m_FlatShader;
  std::shared_ptr<ShaderData> mPickShader;
  std::shared_ptr<ShaderData> m_GridShader;
  std::shared_ptr<InputManager> m_InputManager;

  std::shared_ptr<IEngineCamera> mEngineCamera;

private:
  bool isInitialized = false;
  bool bFlatShade = false;
  bool bDebugGBuffer = false;
  int mDebugBufferIndex = 0;
  bool bDrawWireFrameMode = false;
  bool bDrawGrid = true;
  std::string m_CurrentScenePath;
};
