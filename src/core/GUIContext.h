#pragma once

struct GLFWwindow;

// Event handling:
//     You should discard mouse/keyboard 
//     messages in your game/engine when 
//     io.WantCaptureMouse/io.WantCaptureKeyboard are set.
//

class GUIContext {
public:
  GUIContext(GLFWwindow* window);
  ~GUIContext();

public:
  void BeginFrame();
  void Render();
  void Shutdown();
};
