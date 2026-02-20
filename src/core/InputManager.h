#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "Input.h"

typedef struct GLFWwindow GLFWwindow;

class InputManager {
public:
  InputManager(GLFWwindow *window);
  void Update();

  bool GetKey(int key);
  bool GetKeyString(const std::string& key_str);
  bool GetKeyDown(int key);
  bool GetKeyDownString(const std::string& key_str);
  bool GetKeyUp(int key);
  bool GetKeyUpString(const std::string& key_str);
private:
  GLFWwindow *mWindow = nullptr;

  bool keyState[GLFW_KEY_LAST + 1];
  bool prevKeyState[GLFW_KEY_LAST + 1];
};
