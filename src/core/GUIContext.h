#pragma once

#include "MenuBar.h"

#include <memory>
#include <vector>

#include "EditorPanel.h"
#include <iostream>

struct GLFWwindow;

class GUIContext {
public:
  // Resource aquisition is initialization
  GUIContext(GLFWwindow *window);
  ~GUIContext();

public:
  void BeginFrame();
  void Render();
  void Shutdown();

  void RenderEditorPanels();

  // THIS IS NOT SUSTAINABLE
  template <typename PanelType, typename... Args>
  PanelType *AddPanel(Args &&...args) {
    auto panel = std::make_unique<PanelType>(std::forward<Args>(args)...);
    PanelType *ptr = panel.get();
    m_Panels.push_back(std::move(panel));
    std::cout << "Getting panel" << std::endl;
    ptr->OnInit();
    return ptr;
  }

private:
  MenuBar m_MenuBar;
  void DrawToolbar();

private:
  std::vector<std::unique_ptr<EditorPanel>> m_Panels;
};
