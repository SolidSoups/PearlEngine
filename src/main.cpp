
// src
#include "PearlEngine.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
  PearlEngine* engine = new PearlEngine;
  if (!engine->IsInitialized()) {
    return -1;
  }

  engine->RunUpdateLoop();
  delete engine;
  glfwTerminate();
  return 0;
}
