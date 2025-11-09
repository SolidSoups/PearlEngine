#include "ViewportEditorPanel.h"

#include <imgui.h>

#include "FrameBuffer.h"
#include "Renderer.h"

ViewportEditorPanel::ViewportEditorPanel(Framebuffer *framebuffer)
    : EditorPanel("Viewport"), m_Framebuffer(framebuffer), m_Size(0.0f, 0.0f),
      m_WasResized(false) {}

void ViewportEditorPanel::OnImGuiRender() {
  if (!m_IsOpen) return;

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);

  // Get available content region
  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

  // Check if resized
  m_WasResized = false;
  if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
    if (viewportPanelSize.x != m_Size.x || viewportPanelSize.y != m_Size.y) {
      m_Size = {viewportPanelSize.x, viewportPanelSize.y};
      m_WasResized = true;
    }
  }

  // display the framebuffer
  ImGui::Image(
    (void*)(intptr_t)m_Framebuffer->GetTextureID(),
    viewportPanelSize,
    ImVec2(0, 1), ImVec2(1, 0)
  );

  // Capture input state
  m_IsHovered = ImGui::IsItemHovered();
  m_IsFocused = ImGui::IsWindowFocused();

  // Get mouse delta when hovering and clicking
  if(m_IsHovered){
    ImGuiIO& io = ImGui::GetIO();
    m_ScrollDelta = io.MouseWheel;
    m_MouseDelta = glm::vec2(io.MouseDelta.x, io.MouseDelta.y);
    m_RightMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);
    m_MiddleMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Middle);
  } else{
    m_MouseDelta = glm::vec2(0.0f);
    m_ScrollDelta = 0.0f;
    m_RightMouseDown = false;
    m_MiddleMouseDown = false;
  }

  ImGui::End();
}
