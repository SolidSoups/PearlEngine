#pragma once

#include "PearlWindow.h"
#include "Shader.h"
#include "Cube.h"
#include "Camera.h"

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
  PearlWindow pwin{800, 800, "LearnOpenGL"};

  Shader myShader{"shaders/vert.glsl", "shaders/frag.glsl"};
  Cube myCube{};

  Camera mainCamera{};

private:
  bool isInitialized = false;
};
