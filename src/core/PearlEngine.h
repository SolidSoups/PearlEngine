#pragma once

#include "EngineContext.h"
#include "GUIContext.h"
#include "PearlWindow.h"
#include "Camera.h"
#include "CameraController.h"
#include "FrameBuffer.h"
#include "Scene.h"
#include "ShaderData.h"
#include "ServiceLocator.h"
#include "MessageQueue.h"
#include "ViewportEditorPanel.h"

#include <memory>

struct GLFWwindow;

class PearlEngine{
public:
  PearlEngine();
  ~PearlEngine();

  void Initialize();
  void RunUpdateLoop();
  void Update();
  void Render();
  void RenderEditor();

  inline bool IsInitialized() { return isInitialized; }
private:
  void ProcessInput(GLFWwindow* window);

private:
  void AddMenuBarItems();
  
public:
  // RaII rules 👑
  PearlWindow pwin{1800, 1000, "PearlEngine"};
  GUIContext m_GUIContext{pwin.GetWindow()};
  MessageQueue m_MessageQueue{};


  // framebuffer
  std::unique_ptr<Framebuffer> m_ViewportFramebuffer;
  glm::vec2 m_ViewportSize{1280, 70};

  Scene m_Scene;

  Camera m_Camera{};
  std::unique_ptr<CameraController> m_CameraController;

  ShaderHandle m_ShaderHandle;

  ViewportEditorPanel* m_ViewportPanel = nullptr; // needed here

  EngineContext m_EngineContext{m_Scene, m_MessageQueue};
  ServiceLocator m_ServiceLocator;
private:
  bool isInitialized = false;
};
