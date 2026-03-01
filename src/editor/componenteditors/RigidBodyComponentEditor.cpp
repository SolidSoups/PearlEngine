#include "RigidBodyComponentEditor.h"

#include <imgui.h>

void RigidBodyComponentEditor::OnDrawComponent(void* target, ecs::Entity entity){
  auto* rbComp = static_cast<RigidBodyComponent*>(target);
  if(!rbComp) return;

  constexpr float labelWidth = 80.f;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Mass");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##Mass", &rbComp->mass, 0.1f);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Gravity");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##Gravity", &rbComp->gravity, 0.1f);
}


