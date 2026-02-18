#include "ViewportEditorPanel.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FrameBuffer.h"
#include "MenuRegistry.h"
#include "Renderer.h"

#include "SelectionWizard.h"
#include "ServiceLocator.h"
#include "Scene.h"

#include "Camera.h"
#include "CameraData.h"
#include "TransformComponent.h"

ViewportEditorPanel::ViewportEditorPanel(Framebuffer *framebuffer)
    : EditorPanel("Viewport"), m_Framebuffer(framebuffer), m_Size(0.0f, 0.0f),
      m_WasResized(false) {
  MenuRegistry::Get().Register("Windows/Viewport", &m_IsOpen);
}

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
  ImVec2 viewportMin = ImGui::GetCursorScreenPos();
  ImGui::Image(
    (void*)(intptr_t)m_Framebuffer->GetTextureID(),
    viewportPanelSize,
    ImVec2(0, 1), ImVec2(1, 0)
  );

  // Gizmos
  if(SelectionWizard::HasSelection()){
    auto selectedEntity = SelectionWizard::Get(); 
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(viewportMin.x, viewportMin.y, viewportPanelSize.x, viewportPanelSize.y);

    Scene& scene = ServiceLocator::Get<Scene>();
    ecs::Coordinator& coordinator = scene.GetCoordinator();

    // Camera
    CameraData* camData = ServiceLocator::Get<Camera>().GetCurrentTarget();
    glm::mat4 cameraView = camData->GetViewMatrix();
    glm::mat4 cameraProjection = camData->GetProjectionMatrix();

    // Entity transform
    auto& entityTransform = coordinator.GetComponent<TransformComponent>(selectedEntity);
    glm::mat4 transform = entityTransform.GetModelMatrix();

    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
                         ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL, glm::value_ptr(transform));

    if(ImGuizmo::IsUsing()){
      glm::vec3 translation, rotation, scale;
      ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

      entityTransform.position = translation;
      entityTransform.rotation = rotation;
      entityTransform.scale = scale;
    }
  }


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
