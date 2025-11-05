#pragma once

#include "PearlWindow.h"
#include "Shader.h"
#include "Triangle.h"
#include "Square.h"

struct GLFWwindow;

class PearlEngine{
public:
  PearlEngine();
  ~PearlEngine();

  void RunUpdateLoop();

  inline bool IsInitialized() { return isInitialized; }
private:
  void ProcessInput(GLFWwindow* window);

private:
  PearlWindow pwin{800, 600, "LearnOpenGL"};

  Shader myShader{"shaders/vert.glsl", "shaders/frag.glsl"};
  Triangle myTriangle{};
  Square mySquare{};

private:
  bool isInitialized = false;
};
