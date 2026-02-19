
// src
#include "PearlEngine.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
#ifdef PEARL_HAS_AVX2
  LOG_INFO << "PEARL_HAS_AVX2!";
#endif


  PearlEngine* engine = new PearlEngine;
  if (!engine->IsInitialized()) {
    return -1;
  }


  engine->RunUpdateLoop();
  delete engine;
  glfwTerminate();
  return 0;
}
