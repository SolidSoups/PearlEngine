// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// src
#include "PearlWindow.h"
#include "Shader.h"
#include "PearlEngine.h"

// std
#include <iostream>


int main(){
  PearlEngine engine;
  if(!engine.IsInitialized()){
    return -1;
  }

  engine.RunUpdateLoop();

  return 0;
}


