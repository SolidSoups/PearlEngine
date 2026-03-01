#include "SphereColliderComponentEditor.h"

void SphereColliderComponentEditor::OnDrawComponent(void* target, [[maybe_unused]] ecs::Entity entity){
  SphereColliderComponent* sphereComp = static_cast<SphereColliderComponent*>(target); 
  if(!sphereComp) return;

  constexpr float labelWidth = 80.f;

  // position
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Position");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat3("##Position", &sphereComp->position.x, 0.1f);

  // radius
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Radius");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##Radius", &sphereComp->radius, 0.1f, 0.0f, 0.0f);
}
