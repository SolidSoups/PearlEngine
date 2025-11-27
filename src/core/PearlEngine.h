#pragma once

#include "GUIContext.h"
#include "InspectorEditorPanel.h"
#include "PearlWindow.h"
#include "Camera.h"
#include "CameraController.h"
#include "FrameBuffer.h"
#include "PanelCommon.h"
#include "Scene.h"
#include "ShaderData.h"

#include <memory>
#include <tuple>
#include <vector>

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
  GUIContext imGuiContext{pwin.GetWindow()};

  // framebuffer
  std::unique_ptr<Framebuffer> m_ViewportFramebuffer;
  glm::vec2 m_ViewportSize{1280, 70};

  Scene m_Scene;

  Camera m_Camera{};
  std::unique_ptr<CameraController> m_CameraController;

  ShaderHandle m_ShaderHandle;

  ViewportEditorPanel* m_ViewportPanel = nullptr; // needed here

private:
  bool isInitialized = false;
};
