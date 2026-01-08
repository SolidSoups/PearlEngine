#include "AssetEditorPanel.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "imgui.h"

#include "AssetSystem.h"
#include "ResourceSystem.h"

void AssetEditorPanel::OnImGuiRender() {
  if (!m_IsOpen)
    return;

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);

  for (const auto &asset : r_AssetSystem.GetAssetsDescriptors()) {
    std::string uniqueName = asset.stem + "##" + asset.localPath.string();

    if (ImGui::Selectable(uniqueName.c_str())) {
      // noop, only for right clicks
    }

    if (ImGui::BeginPopupContextItem()) {
      if (ImGui::MenuItem("Load Asset to scene")) {
        LoadAsset(asset);
      }

      ImGui::EndPopup();
    }
  }

  ImGui::End();
}

void AssetEditorPanel::LoadAsset(const pe::AssetDescriptor &assetDesc) {
  if (assetDesc.type == "Mesh_Asset") {
    std::unique_ptr<IAsset> asset = r_AssetSystem.LoadAsset(&assetDesc);
    IResource *resource = r_ResourceSystem.LoadAsset(asset.get());
    Mesh *mesh = dynamic_cast<Mesh *>(resource);
    if (!mesh) {
      LOG_ERROR << "RESOURCE IS NOT MESH";
      return;
    }
    MeshHandle meshHandle = r_ResourceSystem.Create(mesh);
    LOG_INFO << "No segfault 4";

    GameObject *go = r_Scene.CreateGameObject();
    go->AddComponent<RenderComponent>(meshHandle, m_matHandle);
    go->AddComponent<TransformComponent>();
  }
}

void AssetEditorPanel::LoadColors() {}
