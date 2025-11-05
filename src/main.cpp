// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// src
#include "PearlWindow.h"
#include "Shader.h"
#include "Triangle.h"
#include "PearlEngine.h"

// std
#include <iostream>


int main(){
  PearlEngine* engine = new PearlEngine();
  if(!engine->IsInitialized()){
    return -1;
  }

  engine->RunUpdateLoop();

  delete engine;
  return 0;
}


