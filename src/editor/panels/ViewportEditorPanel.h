#pragma once
#include <memory>

#include <glm/glm.hpp>

#include "EditorPanel.h"
#include "FrameBuffer.h"
#include "TextureData.h"

class ViewportEditorPanel : public EditorPanel{
public:
  ViewportEditorPanel(Framebuffer* framebuffer);

  void OnImGuiRender() override;

  glm::vec2 GetSize() const { return m_Size; }
  glm::vec2 GetViewportPos() const { return m_ViewportPos; }
  inline glm::vec2 GetViewportMin() const { return m_ViewportMin;}
  bool IsResized() const { return m_WasResized; }

  // input state
  bool IsHovered() const { return m_IsHovered; }
  bool IsFocused() const { return m_IsFocused; }
  glm::vec2 GetMouseDelta() const { return m_MouseDelta; }
  float GetScrollDelta() const { return m_ScrollDelta; }
  bool IsRightMouseDown() const { return m_RightMouseDown; }
  bool IsMiddleMouseDown() const { return m_MiddleMouseDown; }

  inline bool DidLeftMouseClick() const { return m_LeftMouseClicked; }
  inline glm::vec2 ConsumeLeftMouseClick() {
    m_LeftMouseClicked = false;
    return m_MouseClickPos;
  }
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

  bool m_LeftMouseClicked = false;
  glm::vec2 m_MouseClickPos{0.0f};
  glm::vec2 m_ViewportPos{0.0f};
  glm::vec2 m_ViewportMin{0.0f};


  std::shared_ptr<TextureData> mPlayTexture;
  std::shared_ptr<TextureData> mPauseTexture;
  std::shared_ptr<TextureData> mReloadTexture;
  std::shared_ptr<TextureData> mStopTexture;
};
