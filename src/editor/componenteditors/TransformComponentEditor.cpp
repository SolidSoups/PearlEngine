#include "TransformComponentEditor.h"
#include "imgui.h"

void TransformComponentEditor::OnDrawComponent(Component* target){
  constexpr float labelWidth = 80.f;
  TransformComponent* transform = dynamic_cast<TransformComponent*>(target); 

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
