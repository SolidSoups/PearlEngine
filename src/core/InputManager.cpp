#include "InputManager.h"

#include <sstream>
#include <algorithm>

#include <GLFW/glfw3.h>
#include "Logger.h"

InputManager::InputManager(GLFWwindow *window) { mWindow = window; }

void InputManager::Update() {
  if (!mWindow)
    return;

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


}
bool InputManager::GetKey(int key){
  return keyState[key];
}
bool InputManager::GetKeyString(const std::string& key_str){
  auto key_it = StringToKey.find(key_str);
  if(key_it == StringToKey.end()){
    LOG_ERROR << "Key '" << key_str << "' is invalid";
    return false;
  }
  return keyState[key_it->second];
}
bool InputManager::GetKeyDown(int key){
  return keyState[key] && !prevKeyState[key];
}
bool InputManager::GetKeyDownString(const std::string& key_str){
  auto key_it = StringToKey.find(key_str);
  if(key_it == StringToKey.end()){
    LOG_ERROR << "Key '" << key_str << "' is invalid";
    return false;
  }
  int key = key_it->second;
  return keyState[key] && !prevKeyState[key];
}
bool InputManager::GetKeyUp(int key){
  return !keyState[key] && prevKeyState[key];
}
bool InputManager::GetKeyUpString(const std::string& key_str){
  auto key_it = StringToKey.find(key_str);
  if(key_it == StringToKey.end()){
    LOG_ERROR << "Key '" << key_str << "' is invalid";
    return false;
  }
  int key = key_it->second;
  return !keyState[key] && prevKeyState[key];
}
