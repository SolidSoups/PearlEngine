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

    for (const auto &asset : pe::AssetSystem::Get().GetAssetsDescriptors()) {
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
    if (assetDesc.extension == ".obj") {
        std::unique_ptr<IAsset> asset =
            pe::AssetSystem::Get().LoadAsset(&assetDesc);
        IResource *resource = ResourceSystem::Get().LoadAsset(asset.get());
        Mesh *mesh = dynamic_cast<Mesh *>(resource);
        if (!mesh) {
            LOG_ERROR << "RESOURCE IS NOT MESH";
            return;
        }
        MeshHandle meshHandle = ResourceSystem::Get().Create(mesh);

        GameObject *go = r_Scene.CreateGameObject();
        go->AddComponent<RenderComponent>(meshHandle, m_matHandle);
        go->AddComponent<TransformComponent>();
    }
}

void AssetEditorPanel::LoadColors() {}
