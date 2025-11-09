#include "ImGuiWrapperContext.h"

// idk if i need these
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// but i definitely need these
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

ImGuiWrapperContext::ImGuiWrapperContext(GLFWwindow* window){
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // enable docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true); // second arg installs GLFW callbacks and chain to existing ones
  const char* glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

  std::cout << "ImGuiContext::ImGuiContext() -> ImGui Context initialized!" << std::endl;
}

ImGuiWrapperContext::~ImGuiWrapperContext(){
  Shutdown();
}

void ImGuiWrapperContext::BeginFrame(){

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();
}
void ImGuiWrapperContext::Render(){
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Update and Render additional platform windows
  ImGuiIO& io = ImGui::GetIO();
  if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);

    // TODO for OpenGl: restore current GL context
  }
}
void ImGuiWrapperContext::Shutdown(){
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  std::cout << "ImGuiContext::Shutdown() -> ImGui context shutdown" << std::endl;
}
