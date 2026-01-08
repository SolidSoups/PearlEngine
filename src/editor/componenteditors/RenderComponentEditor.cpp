#include "RenderComponentEditor.h"
#include "AssetDescriptor.h"
#include "AssetSystem.h"
#include "ResourceSystem.h"
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

  std::vector<std::string> meshAssetNames;
  for (const pe::AssetDescriptor &assetd : assets) {
    if (assetd.type == "Mesh_Asset") {
      meshAssetNames.push_back(assetd.stem);
    }
  }

  ImGui::Text("Mesh: ");
  ImGui::SameLine();
  if (ImGui::Button("Select Mesh..##mesh")) {
    ImGui::OpenPopup("##SearchablePopup_mesh_selector");
  }

  int selectedIndex = -1;
  if(SearchablePopup("mesh_selector", "Select a mesh", meshAssetNames, &selectedIndex) && selectedIndex > -1){
    if(RenderComponent* renderComp = dynamic_cast<RenderComponent*>(target)){
      renderComp->SetMeshFromAsset(assets[selectedIndex]);
    }
  }
}
