#include "RenderComponentEditor.h"
#include "imgui.h"

void RenderComponentEditor::OnDrawComponent(Component* target) {
  RenderComponent* renderComp = dynamic_cast<RenderComponent*>(target);

  float labelWidth = 220.f;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Material Handle:");
  ImGui::SameLine(labelWidth);
  int matId = renderComp->materialHandle.id;
  ImGui::SetNextItemWidth(-10);
  ImGui::InputInt("##MaterialHandle", &matId, 0, 0, ImGuiInputTextFlags_ReadOnly);


  ImGui::AlignTextToFramePadding();
  ImGui::Text("Mesh Handle:");
  ImGui::SameLine(labelWidth);
  int meshId = renderComp->meshHandle.id;
  ImGui::SetNextItemWidth(-10);
  ImGui::InputInt("##MeshOldHandle", &meshId, 0, 0, ImGuiInputTextFlags_ReadOnly);
}
