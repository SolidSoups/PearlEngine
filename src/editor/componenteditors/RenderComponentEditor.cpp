#include "RenderComponentEditor.h"
#include "imgui.h"

void RenderComponentEditor::OnDrawComponent(Component *target) {
  RenderComponent *renderComp = dynamic_cast<RenderComponent *>(target);

  float labelWidth = 220.f;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Material:");
  ImGui::SameLine(labelWidth);
  ImGui::SetNextItemWidth(-10);
  std::string matInfo = renderComp->material ? "Valid" : "nullptr";
  ImGui::InputText("##Material", &matInfo[0], matInfo.size(),
                   ImGuiInputTextFlags_ReadOnly);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Mesh:");
  ImGui::SameLine(labelWidth);
  ImGui::SetNextItemWidth(-10);
  std::string meshInfo = renderComp->mesh ? "Valid" : "nullptr";
  ImGui::InputText("##Mesh", &meshInfo[0], meshInfo.size(),
                   ImGuiInputTextFlags_ReadOnly);
}
