#include "RenderComponentEditor.h"
#include "AssetDescriptor.h"
#include "AssetSystem.h"
#include "ResourceSystem.h"
#include "imgui.h"
#include "ServiceLocator.h"
#include "ImGuiHelpers.h"
#include <sys/select.h>

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

  // === Draw a selectable mesh
  std::vector<std::string> meshAssetNames;
  std::vector<std::string> textureAssetNames;
  int iteration = 0;
  for (const pe::AssetDescriptor &assetd : assets) {
    if (assetd.type == "Mesh_Asset") {
      meshAssetNames.push_back(assetd.stem);
    } else if (assetd.type == "Texture_Asset") {
      textureAssetNames.push_back(assetd.stem);
    }
  }

  ImGui::Text("Mesh: ");
  ImGui::SameLine();
  if (ImGui::Button("Select Mesh...##mesh")) {
    ImGui::OpenPopup("##SearchablePopup_mesh_selector");
  }

  // query user for a mesh
  pe::AssetDescriptor selectedAsset;
  if (SearchablePopup<pe::AssetDescriptor>(
          "mesh_selector", "Select a mesh", assets,
          [](pe::AssetDescriptor assetd) { return assetd.stem; },
          [](pe::AssetDescriptor assetd) {
            return assetd.type == "Mesh_Asset";
          },
          selectedAsset)) {
    if (RenderComponent *renderComp = dynamic_cast<RenderComponent *>(target)) {
      LOG_INFO << "Selected asset: " << selectedAsset.stem;
      selectedAsset.Debug();
      renderComp->SetMeshFromAsset(selectedAsset);
    }
  }

  ImGui::Separator();

  ImGui::Text("Texture: ");
  ImGui::SameLine();
  if (ImGui::Button("Select Texture..##texture")) {
    ImGui::OpenPopup("##SearchablePopup_texture_selector");
  }

  if (SearchablePopup<pe::AssetDescriptor>(
          "texture_selector", "Select a texture", assets,
          [](pe::AssetDescriptor desc) { return desc.stem; },
          [](pe::AssetDescriptor desc) { return desc.type == "Texture_Asset"; },
          selectedAsset)) {
    LOG_INFO << "SearchablePopup returned true";
    LOG_INFO << "Selected stem: " << selectedAsset.stem;
    selectedAsset.Debug();
    renderComp->SetMaterialFromTextureAsset(selectedAsset);
  }
}
