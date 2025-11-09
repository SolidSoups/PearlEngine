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

private:
  Framebuffer* m_Framebuffer;
  glm::vec2 m_Size;
  bool m_WasResized = false;
};
