#include "RenderComponentEditor.h"
#include "imgui.h"

void RenderComponentEditor::OnDrawComponent(Component *target) {
  RenderComponent *renderComp = dynamic_cast<RenderComponent *>(target);

  float labelWidth = 180.f;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Material:");
  ImGui::SameLine(labelWidth);
  std::string matInfo = renderComp->material ? "Valid" : "nullptr";
  ImGui::Text("%s", &matInfo[0]);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  ImGui::Button("Set Material...");

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Mesh:");
  ImGui::SameLine(labelWidth);
  std::string meshInfo = renderComp->mesh ? "Valid" : "nullptr";
  ImGui::Text("%s", &meshInfo[0]);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  ImGui::Button("Set Mesh...");
}
