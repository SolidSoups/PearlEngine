#include "CapsuleColliderComponentEditor.h"

#include <imgui.h>

void CapsuleColliderComponentEditor::OnDrawComponent(void *target,
                                                     ecs::Entity entity) {
  auto *capComp = static_cast<CapsuleColliderComponent *>(target);
  if (!capComp)
    return;

  constexpr float labelWidth = 80.f;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Start");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat3("##Start", &capComp->a.x, 0.1f);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("End");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat3("##End", &capComp->b.x, 0.1f);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Radius");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##Radius", &capComp->radius, 0.1f, 0.0f);
}
