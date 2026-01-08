#include "RenderComponentEditor.h"
#include "AssetSystem.h"
#include "imgui.h"
#include "ServiceLocator.h"
#include "ImGuiHelpers.h"

void RenderComponentEditor::OnDrawComponent(Component *target) {
  RenderComponent *renderComp = dynamic_cast<RenderComponent *>(target);

  float labelWidth = 220.f;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Material Handle:");
  ImGui::SameLine(labelWidth);
  int matId = renderComp->materialHandle.id;
  ImGui::SetNextItemWidth(-10);
  ImGui::InputInt("##MaterialHandle", &matId, 0, 0,
                  ImGuiInputTextFlags_ReadOnly);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Mesh Handle:");
  ImGui::SameLine(labelWidth);
  int meshId = renderComp->meshHandle.id;
  ImGui::SetNextItemWidth(-10);
  ImGui::InputInt("##MeshOldHandle", &meshId, 0, 0,
                  ImGuiInputTextFlags_ReadOnly);

  ImGui::Separator();

  auto &assetSystem = ServiceLocator::Get<pe::AssetSystem>();
  const auto &assets = assetSystem.GetAssetsDescriptors();

  std::vector<const pe::AssetDescriptor *> meshAssets;
  for (const pe::AssetDescriptor &assetd : assets) {
    if (assetd.type == "Mesh_Asset") {
      meshAssets.push_back(&assetd);
    }
  }

  ImGui::Text("Mesh: ");
  ImGui::SameLine();
  if (ImGui::Button("Select Mesh..##mesh")) {
    ImGui::OpenPopup("##SearchablePopup_mesh_selector");
  }

  const pe::AssetDescriptor *selectedMesh = nullptr;
  if(SearchablePopup("mesh_selector", meshAssets, 
                     [](const pe::AssetDescriptor* desc) {
    return desc->stem; },
     &selectedMesh)){
    
  }
}
