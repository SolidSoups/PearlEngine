#pragma once

// fwd
struct GLFWwindow;

// handles glfw and glad initialization, manages window
class PearlWindow{
public:
  PearlWindow(int width, int height, const char* title);
  ~PearlWindow();

  PearlWindow(const PearlWindow&) = delete;             // delete copy-constructor
  PearlWindow& operator=(const PearlWindow&) = delete;  // delete copy-assignment
  PearlWindow(PearlWindow&&) = delete;                  // delete move-constructor
  PearlWindow& operator=(PearlWindow&&) = delete;       // delete move-assignment

  inline bool IsInitialized() { return isInitialized; }
  inline GLFWwindow* GetWindow() { return window; }
public:
  unsigned int window_width, window_height;
    
private:
  bool isInitialized = false;
  GLFWwindow* window = nullptr;
};
