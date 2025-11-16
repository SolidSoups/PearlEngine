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
  PearlWindow pwin{1800, 1000, "LearnOpenGL"};
  GUIContext imGuiContext{pwin.GetWindow()};

  // framebuffer
  std::unique_ptr<Framebuffer> m_ViewportFramebuffer;
  glm::vec2 m_ViewportSize{1280, 70};

  Scene m_Scene;

  Camera m_Camera{};
  std::unique_ptr<CameraController> m_CameraController;

  ShaderHandle m_ShaderHandle;

  std::vector<std::unique_ptr<EditorPanel>> m_Panels;
  ViewportEditorPanel* m_ViewportPanel = nullptr;
  SceneHierarchyEditorPanel* m_ScenePanel = nullptr;
  ResourceEditorPanel* m_ResourcePanel = nullptr;
  InspectorEditorPanel* m_InspectorPanel = nullptr;


private:
  bool isInitialized = false;

  template<typename PanelType, typename... Args>
  PanelType* AddPanel(Args&&... args){
    auto panel = std::make_unique<PanelType>(std::forward<Args>(args)...);
    PanelType* ptr = panel.get();
    m_Panels.push_back(std::move(panel));
    return ptr;
  }
};
