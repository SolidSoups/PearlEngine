#pragma once

struct GLFWwindow;

// Event handling:
//     You should discard mouse/keyboard 
//     messages in your game/engine when 
//     io.WantCaptureMouse/io.WantCaptureKeyboard are set.
//

class ImGuiWrapperContext {
public:
  ImGuiWrapperContext(GLFWwindow* window);
  ~ImGuiWrapperContext();

public:
  void BeginFrame();
  void Render();
  void Shutdown();
};
