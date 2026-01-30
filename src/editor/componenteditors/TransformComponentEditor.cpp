#include "TransformComponentEditor.h"
#include "Logger.h"
#include "imgui.h"

void TransformComponentEditor::OnDrawComponent(void* target, [[maybe_unused]] ecs::Entity entity){
  constexpr float labelWidth = 80.f;
  TransformComponent* transform = static_cast<TransformComponent*>(target);
  if (!transform) return;

  // position
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Position");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat3("##Position", &transform->position.x, 0.1f);

  // rotation
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Rotation");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat3("##Rotation", &transform->rotation.x, 0.5f);

  // scale
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Scale");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat3("##Scale", &transform->scale.x, 0.1f, 0.001f);
}
