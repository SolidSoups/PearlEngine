#pragma once

#include <memory>
#include "RuntimeState.h"
#ifdef PEARL_EDITOR
#include "PickingFramebuffer.h"
#include "GUIContext.h"
#endif
#include "PearlWindow.h"
#include "GBuffer.h"
#include "Mesh.h"
#include "Camera.h"
#include "ShaderData.h"
#include "Scene.h"
#include "MessageBus.h"
#include "MessageQueue.h"

#ifdef PEARL_EDITOR
class ViewportEditorPanel;
#endif

class Framebuffer;
class CameraController;
class InputManager;

class MeshManager;
class TextureManager;
class ShaderManager;

struct GLFWwindow;

class PearlEngine {
private:
#ifdef PEARL_EDITOR
  RuntimeState mRuntimeState = EDITOR;
#else
  RuntimeState mRuntimeState = RUNTIME;
#endif
public:
  PearlEngine();
  ~PearlEngine();

  void Initialize();
  void RunUpdateLoop();
  void Update();
  void Render();
#ifdef PEARL_EDITOR
  void RenderEditor();
#endif

  void GeometryRenderPass();
#ifdef PEARL_EDITOR
  void QuadDebugRenderPass();
  void FlatShadePass();
#endif
  void LightingPass();

  // specialized render passes
#ifdef PEARL_EDITOR
  void PickingRenderPass();
  uint32_t ReadPickedVal(unsigned int x, unsigned int y);
#endif

  inline bool IsInitialized() { return isInitialized; }

private:
  void ProcessInput(GLFWwindow *window);

#ifdef PEARL_EDITOR
private:
  void AddMenuBarItems();
#endif

private:
  std::unique_ptr<Scene> mScene;
  std::unique_ptr<MessageBus> m_MessageBus;
  std::unique_ptr<MessageQueue> m_MessageQueue;

  std::unique_ptr<MeshManager> m_MeshManager;
  std::unique_ptr<TextureManager> m_TextureManager;
  std::unique_ptr<ShaderManager> m_ShaderManager;

public:
  PearlWindow pwin{1800, 1000, "PearlEngine"};
#ifdef PEARL_EDITOR
  GUIContext m_GUIContext{pwin.GetWindow()};
#endif

  // framebuffer
  std::unique_ptr<Framebuffer> m_ViewportFramebuffer;
  glm::vec2 m_ViewportSize{1280, 70};
#ifdef PEARL_EDITOR
  ViewportEditorPanel *m_ViewportPanel = nullptr; // needed here
  std::unique_ptr<PickingFramebuffer> mPickFramebuf;
#endif

  // Gbuffer stuff
  std::unique_ptr<GBuffer> m_GBuffer;
  std::unique_ptr<Mesh> m_FullscreenQuad;
  std::unique_ptr<Mesh> m_WorldPlaneQuad;

  std::shared_ptr<InputManager> m_InputManager;
  std::shared_ptr<ShaderData> m_GeometryShader;
  std::shared_ptr<ShaderData> m_DisplayShader;
  std::shared_ptr<ShaderData> m_LightShader;
#ifdef PEARL_EDITOR
  std::shared_ptr<ShaderData> m_FlatShader;
  std::shared_ptr<ShaderData> mPickShader;
  std::shared_ptr<ShaderData> m_GridShader;
#endif

  std::shared_ptr<IEngineCamera> mEngineCamera;

private:
  bool isInitialized = false;
#ifdef PEARL_EDITOR
  bool bFlatShade = false;
  bool bDebugGBuffer = false;
  int mDebugBufferIndex = 0;
  bool bDrawWireFrameMode = false;
  bool bDrawGrid = true;
  std::string m_CurrentScenePath;
#endif
};
