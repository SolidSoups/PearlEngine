// src
#include "PearlEngine.h"

// lib
#include <glad/glad.h>
#include "GLFW/glfw3.h"

// std
#include <iostream>

PearlEngine::PearlEngine(){
  if(!pwin.IsInitialized()){
    isInitialized = false;
    std::cout << "PearlEngine::PearlEngine() -> Engine failed to construct and initialize" << std::endl;
    return;
  }

  isInitialized = true;
  std::cout << "PearlEngine::PearlEngine() -> Engine constructed and initialized" << std::endl;
}

PearlEngine::~PearlEngine(){
  // nothing to do here yet 
  std::cout << "PearlEngine::~PearlEngine() -> Engine deconstructing" << std::endl;
}

// b@UPDATE
void PearlEngine::RunUpdateLoop(){
  GLFWwindow* window = pwin.GetWindow();

  while(!glfwWindowShouldClose(window)){
    ProcessInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mySquare.Render(myShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void PearlEngine::ProcessInput(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }
}
