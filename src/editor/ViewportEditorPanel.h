#pragma once
#include "EditorPanel.h"
#include "FrameBuffer.h"
#include <glm/glm.hpp>

class ViewportEditorPanel : public EditorPanel{
public:
  ViewportEditorPanel(Framebuffer* framebuffer);

  void OnImGuiRender() override;

  glm::vec2 GetSize() const { return m_Size; }
  bool IsResized() const { return m_WasResized; }

  // input state
  bool IsHovered() const { return m_IsHovered; }
  bool IsFocused() const { return m_IsFocused; }
  glm::vec2 GetMouseDelta() const { return m_MouseDelta; }
  float GetScrollDelta() const { return m_ScrollDelta; }
  bool IsRightMouseDown() const { return m_RightMouseDown; }
  bool IsMiddleMouseDown() const { return m_MiddleMouseDown; }

private:
  Framebuffer* m_Framebuffer;
  glm::vec2 m_Size;
  bool m_WasResized = false;
  bool m_IsHovered = false;
  bool m_IsFocused = false;
  glm::vec2 m_MouseDelta{0.0f, 0.0f};
  float m_ScrollDelta = 0.0f;
  bool m_RightMouseDown = false;
  bool m_MiddleMouseDown = false;
};
