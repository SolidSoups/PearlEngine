#include "PearlWindow.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>

// this callback must be static and not a part of a class
static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}

PearlWindow::PearlWindow(int width, int height, const char* title){
  // initialize and give hints
  glfwInit();  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create the actual window
  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if(window == NULL){
    std::cout << "PearlWindow::PearlWindow(..) -> Failed to create GLFW window" << std::endl;
    glfwTerminate();
    isInitialized = false;
    return;
  }
  glfwMakeContextCurrent(window);
  

  // initialize glad
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "PearlWindow::PearlWindow(..) -> Failed to initialize GLAD" << std::endl;
    isInitialized = false;
    glfwTerminate();
    return;
  }

  // tell OpenGL the size of the rendering window, and register callback for it
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  isInitialized = true;
  std::cout << "PearlWindow::PearlWindow(..) -> Succesfully initialized window" << std::endl;
}

PearlWindow::~PearlWindow(){
  std::cout << "PearlWindow::~PearlWindow() -> Terminating Window" << std::endl;
  glfwTerminate();
}

