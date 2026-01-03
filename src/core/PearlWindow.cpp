#include <iostream>

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "PearlWindow.h"
#include "PearlEngine.h"
#include "ServiceLocator.h"
#include "Logger.h"

// this callback must be static and not a part of a class
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);

  auto* locator = static_cast<ServiceLocator*>(glfwGetWindowUserPointer(window));
  if(locator){
    float newAspect = (float)width / (float)height;
    if(locator->IsReady<Camera>()){
      locator->Get<Camera>().SetAspectRatio(newAspect);
    }

    // update window dimensions
    if(locator->IsReady<PearlWindow>()){
      auto& pwin = locator->Get<PearlWindow>();
      pwin.window_width = width;
      pwin.window_height = height;
    }
  }
}

PearlWindow::PearlWindow(int width, int height, const char* title, ServiceLocator* locator)
  : window_width(width), window_height(height), r_Camera(locator)
{
  // initialize and give hints
  glfwInit();  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

  // create the actual window
  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if(window == NULL){
    LOG_INFO << "Failed to create GLFW window";
    glfwTerminate();
    isInitialized = false;
    return;
  }
  glfwMakeContextCurrent(window);
  

  // initialize glad
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    LOG_INFO << "Failed to initialize GLAD";
    isInitialized = false;
    glfwTerminate();
    return;
  }

  // tell OpenGL the size of the rendering window, and register callback for it
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwMakeContextCurrent(window);
  isInitialized = true;
  LOG_INFO << "Succesfully initialized window";
  LOG_INFO << "r_Camera state: " << +r_Camera.GetState();
}

PearlWindow::~PearlWindow(){
  LOG_INFO << "Terminating Window";
  if(window != nullptr){
    glfwDestroyWindow(window);
  }
  LOG_INFO << "PearlWindow terminated!";
}

