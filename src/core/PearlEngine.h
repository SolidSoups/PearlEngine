#pragma once

#include "ImGuiWrapperContext.h"
#include "PearlWindow.h"
#include "Shader.h"
#include "Cube.h"
#include "Camera.h"
#include "FrameBuffer.h"

#include <memory>

struct GLFWwindow;

class PearlEngine{
public:
  PearlEngine();
  ~PearlEngine();

  void RunUpdateLoop();

  inline bool IsInitialized() { return isInitialized; }
private:
  void ProcessInput(GLFWwindow* window);

public:
  // RaII rules 👑
  PearlWindow pwin{1800, 1000, "LearnOpenGL"};
  ImGuiWrapperContext imGuiContext{pwin.GetWindow()};

  // framebuffer
  std::unique_ptr<Framebuffer> m_ViewportFramebuffer;
  glm::vec2 m_ViewportSize{1280, 70};


  Shader myShader{"shaders/vert.glsl", "shaders/frag.glsl"};
  Cube myCube{};

  Camera mainCamera{};

private:
  bool isInitialized = false;
};
