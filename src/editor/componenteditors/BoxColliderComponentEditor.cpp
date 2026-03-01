#include "BoxColliderComponentEditor.h"

#include <imgui.h>

void BoxColliderComponentEditor::OnDrawComponent(void* target, ecs::Entity entity){
  auto* boxComp = static_cast<BoxColliderComponent*>(target);
  if(!boxComp) return;

  constexpr float labelWidth = 80.f;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Center");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat3("##Center", &boxComp->center.x, 0.1f);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Size");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat3("##Size", &boxComp->size.x, 0.1f, 0.0f);
}


