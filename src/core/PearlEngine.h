#pragma once

#include "GUIContext.h"
#include "PearlWindow.h"
#include "Cube.h"
#include "Camera.h"
#include "CameraController.h"
#include "FrameBuffer.h"
#include "EditorPanel.h"
#include "ViewportEditorPanel.h"
#include "Scene.h"

#include <memory>
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
  std::unique_ptr<Material> m_SunshineMaterial;
  std::unique_ptr<Material> m_PearlMaterial;

  std::vector<std::unique_ptr<EditorPanel>> m_Panels;
  ViewportEditorPanel* m_ViewportPanel = nullptr;


private:
  bool isInitialized = false;
};
