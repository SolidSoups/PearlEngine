#pragma once


#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "RuntimeState.h"
#include "EditorPanel.h"
#include "MenuBar.h"
#include "TextureData.h"

struct GLFWwindow;

class GUIContext {
public:
  // Resource aquisition is initialization
  GUIContext(GLFWwindow *window);
  ~GUIContext();

public:
  void BeginFrame(RuntimeState runtimeState);
  void Render();
  void Shutdown();

  void RenderEditorPanels();

  void SetToolbarCallbacks(
    std::function<void()> onPlayClicked,
    std::function<void()> onPauseClicked,
    std::function<void()> onStopClicked,
    std::function<void()> onReloadClicked,
    std::shared_ptr<TextureData>& playTex,
    std::shared_ptr<TextureData>& pauseTex,
    std::shared_ptr<TextureData>& stopTex,
    std::shared_ptr<TextureData>& reloadTex
  );

  // THIS IS NOT SUSTAINABLE
  template <typename PanelType, typename... Args>
  PanelType *AddPanel(Args &&...args) {
    auto panel = std::make_unique<PanelType>(std::forward<Args>(args)...);
    PanelType *ptr = panel.get();
    m_Panels.push_back(std::move(panel));
    ptr->OnInit();
    return ptr;
  }

private:
  MenuBar m_MenuBar;
  void DrawToolbar(RuntimeState runtimeState);

private:
  std::vector<std::unique_ptr<EditorPanel>> m_Panels;
};
