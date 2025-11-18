
// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// src
#include "PearlEngine.h"


int main(){
  PearlEngine engine;
  if(!engine.IsInitialized()){
    return -1;
  }

  engine.RunUpdateLoop();

  return 0;
}


