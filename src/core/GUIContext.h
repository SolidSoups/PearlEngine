#pragma once

#include "MenuBar.h"
struct GLFWwindow;

class GUIContext {
public:
  GUIContext(GLFWwindow* window);
  ~GUIContext();

public:
  void BeginFrame();
  void Render();
  void Shutdown();

private:
  MenuBar m_MenuBar;
  void DrawToolbar();
};
