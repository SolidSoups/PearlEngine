#include "ViewportEditorPanel.h"

#include <string>

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
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
  if (!m_IsOpen)
    return;

  ImGui::Begin(m_Name.c_str(), &m_IsOpen, ImGuiWindowFlags_MenuBar);

  // draw menu bar
  static int selectedItem = 0;
  static const ImGuizmo::OPERATION operations[] = {
      ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::SCALE};
  static const char *items[] = {"Translate", "Rotate", "Scale"};

  if (ImGui::BeginMenuBar()) {
    ImGui::SetNextItemWidth(90.0f);
    ImGui::Combo("##GizmoMode", &selectedItem, items, IM_ARRAYSIZE(items));
    ImGui::EndMenuBar();
  }

  // add mode keybinds for the viewport
  if (m_IsHovered) {
    if (ImGui::IsKeyPressed(ImGuiKey_T))
      selectedItem = 0;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
      selectedItem = 1;
    if (ImGui::IsKeyPressed(ImGuiKey_S))
      selectedItem = 2;
  }


  // set the selected operation
  ImGuizmo::OPERATION selectedOperation = operations[selectedItem];

  // Get available content region, check resize
  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  m_WasResized = false;
  if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
    if (viewportPanelSize.x != m_Size.x || viewportPanelSize.y != m_Size.y) {
      m_Size = {viewportPanelSize.x, viewportPanelSize.y};
      m_WasResized = true;
    }
  }

  // display the framebuffer
  ImVec2 viewportMin = ImGui::GetCursorScreenPos();
  ImGui::Image((void *)(intptr_t)m_Framebuffer->GetTextureID(),
               viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

  // Render gizmo grid
  Scene &scene = ServiceLocator::Get<Scene>();
  ecs::Coordinator &coordinator = scene.GetCoordinator();

  ImGuizmo::SetOrthographic(false);
  ImGuizmo::SetDrawlist();
  ImGuizmo::SetRect(viewportMin.x, viewportMin.y, viewportPanelSize.x,
                    viewportPanelSize.y);

  CameraData *camData = ServiceLocator::Get<Camera>().GetCurrentTarget();
  glm::mat4 cameraView = camData->GetViewMatrix();
  glm::mat4 cameraProjection = camData->GetProjectionMatrix();
   
  glm::mat4 identity(1.0f);
  // ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(identity), 100.0f);

  // Render Gizmos
  if (SelectionWizard::HasSelection()) {
    auto selectedEntity = SelectionWizard::Get();

    // Entity transform
    auto &entityTransform =
        coordinator.GetComponent<TransformComponent>(selectedEntity);
    glm::mat4 transform = entityTransform.GetModelMatrix();

    ImGuizmo::Manipulate(glm::value_ptr(cameraView),
                         glm::value_ptr(cameraProjection), selectedOperation,
                         ImGuizmo::LOCAL, glm::value_ptr(transform));

    if (ImGuizmo::IsUsing()) {
      if (selectedOperation == ImGuizmo::OPERATION::TRANSLATE) {
        entityTransform.position = glm::vec3(transform[3]);
      } else if (selectedOperation == ImGuizmo::OPERATION::ROTATE) {
        // Normalize columns to remove scale
        glm::vec3 c0 = glm::normalize(glm::vec3(transform[0]));
        glm::vec3 c1 = glm::normalize(glm::vec3(transform[1]));
        glm::vec3 c2 = glm::normalize(glm::vec3(transform[2]));

        // Extract XYZ Euler angles matching GetModelMatrix (Rx * Ry * Rz)
        float y = asinf(glm::clamp(c2.x, -1.0f, 1.0f));
        float x, z;
        if (c2.x < 0.9999f) {
          x = atan2f(-c2.y, c2.z);
          z = atan2f(-c1.x, c0.x);
        } else {
          // Gimbal lock (y = ±90°)
          x = atan2f(c0.y, c1.y);
          z = 0.0f;
        }
        entityTransform.rotation = glm::degrees(glm::vec3(x, y, z));
      } else if (selectedOperation == ImGuizmo::OPERATION::SCALE) {
        entityTransform.scale = glm::vec3(glm::length(glm::vec3(transform[0])),
                                          glm::length(glm::vec3(transform[1])),
                                          glm::length(glm::vec3(transform[2])));
      }
    }
  }

  // Capture input state
  m_IsHovered = ImGui::IsItemHovered();
  m_IsFocused = ImGui::IsWindowFocused();

  // Get mouse delta when hovering and clicking
  if (m_IsHovered) {
    ImGuiIO &io = ImGui::GetIO();
    m_ScrollDelta = io.MouseWheel;
    m_MouseDelta = glm::vec2(io.MouseDelta.x, io.MouseDelta.y);
    m_RightMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);
    m_MiddleMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Middle);
  } else {
    m_MouseDelta = glm::vec2(0.0f);
    m_ScrollDelta = 0.0f;
    m_RightMouseDown = false;
    m_MiddleMouseDown = false;
  }

  ImGui::End();
}
