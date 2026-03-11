#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "Input.h"
#include <glm/glm.hpp>

typedef struct GLFWwindow GLFWwindow;

class InputManager {
public:
  InputManager(GLFWwindow *window);
  void Update();

  bool GetKey(int key) const;
  bool GetKeyString(const std::string& key_str) const;
  bool GetKeyDown(int key) const;
  bool GetKeyDownString(const std::string& key_str) const;
  bool GetKeyUp(int key) const;
  bool GetKeyUpString(const std::string& key_str) const;

  bool GetMouseKey(int key) const;
  bool GetMouseKey(const std::string& key) const;
  bool GetMouseKeyDown(int key) const;
  bool GetMouseKeyDown(const std::string& key) const;
  bool GetMouseKeyUp(int key) const;
  bool GetMouseKeyUp(const std::string& key) const;

  const glm::vec2& GetMouseDelta() const;
  const glm::vec2& GetMousePosition() const;
  float GetScrollDelta() const;
private:
  GLFWwindow *mWindow = nullptr;

  bool keyState[GLFW_KEY_LAST + 1];
  bool prevKeyState[GLFW_KEY_LAST + 1];
  bool mouseKeyState[GLFW_MOUSE_BUTTON_LAST + 1];
  bool prevMouseKeyState[GLFW_MOUSE_BUTTON_LAST + 1];

  glm::vec2 mouseDelta{0.0f};
  glm::vec2 lastMousePos{0.0f};
  
  float scrollDelta{0.0f};
  float prevScrollDelta{0.0f};
  static GLFWscrollfun prevScrollFunction;
};
