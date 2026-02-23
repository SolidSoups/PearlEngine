#include "InputManager.h"

#include <sstream>
#include <algorithm>

#include <imgui/backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "Logger.h"

static float sScrollDelta = 0.0f;
GLFWscrollfun InputManager::prevScrollFunction = nullptr;

const glm::vec2& InputManager::GetMouseDelta() const{
  return mouseDelta;
}

float InputManager::GetScrollDelta() const{
  return scrollDelta;
}

InputManager::InputManager(GLFWwindow *window) { 
  mWindow = window; 

  prevScrollFunction = glfwSetScrollCallback(mWindow, [](GLFWwindow* w, double x, double y){
      if(prevScrollFunction) prevScrollFunction(w, x, y);
      sScrollDelta += (float)y;
  });
}

void InputManager::Update() {
  if (!mWindow)
    return;

  // track mouse delta
  double mouseX, mouseY;  
  glfwGetCursorPos(mWindow, &mouseX, &mouseY);
  mouseDelta.x = mouseX - lastMousePos.x;
  mouseDelta.y = mouseY - lastMousePos.y;
  lastMousePos = {mouseX, mouseY};

  // track scroll delta
  prevScrollDelta = scrollDelta;
  scrollDelta = sScrollDelta - prevScrollDelta;

  // track key state
  for(auto& it : KeyToString){
    int key = it.first;
    if(glfwGetKey(mWindow, key) == GLFW_PRESS){
      prevKeyState[key] = keyState[key];
      keyState[key] = true;
    }
    else if(glfwGetKey(mWindow, key) == GLFW_RELEASE){
      prevKeyState[key] = keyState[key];
      keyState[key] = false;
    }
  }

  // track mouse state
  for(auto& it : MouseKeyToString){
    int key = it.first;
    if(glfwGetMouseButton(mWindow, key) == GLFW_PRESS){
      prevMouseKeyState[key] = mouseKeyState[key];
      mouseKeyState[key] = true;
    }
    else if(glfwGetMouseButton(mWindow, key) == GLFW_RELEASE){
      prevMouseKeyState[key] = mouseKeyState[key];
      mouseKeyState[key] = false;
    }
  }

}
bool InputManager::GetKey(int key) const{
  return keyState[key];
}
bool InputManager::GetKeyString(const std::string& key_str) const{
  auto key_it = StringToKey.find(key_str);
  if(key_it == StringToKey.end()){
    LOG_ERROR << "Key '" << key_str << "' is invalid";
    return false;
  }
  return keyState[key_it->second];
}
bool InputManager::GetKeyDown(int key) const{
  return keyState[key] && !prevKeyState[key];
}
bool InputManager::GetKeyDownString(const std::string& key_str) const{
  auto key_it = StringToKey.find(key_str);
  if(key_it == StringToKey.end()){
    LOG_ERROR << "Key '" << key_str << "' is invalid";
    return false;
  }
  int key = key_it->second;
  return keyState[key] && !prevKeyState[key];
}
bool InputManager::GetKeyUp(int key) const{
  return !keyState[key] && prevKeyState[key];
}
bool InputManager::GetKeyUpString(const std::string& key_str) const{
  auto key_it = StringToKey.find(key_str);
  if(key_it == StringToKey.end()){
    LOG_ERROR << "Key '" << key_str << "' is invalid";
    return false;
  }
  int key = key_it->second;
  return !keyState[key] && prevKeyState[key];
}



bool InputManager::GetMouseKey(int key) const{
  return mouseKeyState[key];
}
bool InputManager::GetMouseKey(const std::string& key_str) const{
  auto key_it = StringToMouseKey.find(key_str);
  if(key_it == StringToMouseKey.end()){
    LOG_ERROR << "Mouse Key '" << key_str << "' is invalid";
    return false;
  }
  int key = key_it->second;
  return GetMouseKey(key);
}
bool InputManager::GetMouseKeyDown(int key) const{
  return mouseKeyState[key] && !prevMouseKeyState[key];	
}
bool InputManager::GetMouseKeyDown(const std::string& key_str) const{
  auto key_it = StringToMouseKey.find(key_str);
  if(key_it == StringToMouseKey.end()){
    LOG_ERROR << "Mouse Key '" << key_str << "' is invalid";
    return false;
  }
  int key = key_it->second;
  return GetMouseKeyDown(key);
}
bool InputManager::GetMouseKeyUp(int key) const{
  return !mouseKeyState[key] && prevMouseKeyState[key];	
}
bool InputManager::GetMouseKeyUp(const std::string& key_str) const{
  auto key_it = StringToMouseKey.find(key_str);
  if(key_it == StringToMouseKey.end()){
    LOG_ERROR << "Mouse Key '" << key_str << "' is invalid";
    return false;
  }
  int key = key_it->second;
  return GetMouseKeyUp(key);
}
