#pragma once

#include "GUIContext.h"
#include "PearlWindow.h"
#include "Camera.h"
#include "CameraController.h"
#include "FrameBuffer.h"
#include "Scene.h"
#include "ShaderData.h"
#include "ServiceLocator.h"
#include "ViewportEditorPanel.h"
#include "AssetSystem.h"
#include "ResourceSystem.h"
#include "SelectionWizard.h"
#include "MessageBus.h"
#include "MessageQueue.h"

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
  ShaderHandle m_ShaderHandle;

public:
  Scene m_Scene;
  Camera m_Camera{};
  std::unique_ptr<CameraController> m_CameraController;
  pe::AssetSystem m_AssetSystem{};
  ResourceSystem m_ResourceSystem{};

private:
  std::unique_ptr<SelectionWizard> m_SelectionWizard;
  std::unique_ptr<MessageBus> m_MessageBus;
  std::unique_ptr<MessageQueue> m_MessageQueue;

public:
  PearlWindow pwin{1800, 1000, "PearlEngine"};
  GUIContext m_GUIContext{pwin.GetWindow()};

  // framebuffer
  std::unique_ptr<Framebuffer> m_ViewportFramebuffer;
  glm::vec2 m_ViewportSize{1280, 70};
  ViewportEditorPanel* m_ViewportPanel = nullptr; // needed here

private:
  bool isInitialized = false;
};
