#include "AssetEditorPanel.h"

#include <filesystem>

#include "MeshData.h"
#include "RenderComponentEditor.h"
#include "TransformComponentEditor.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"

#include "Project.h"
#include "ResourceSystem.h"
#include "meshLoaders.h"
#include "AssetSystem.h"

void AssetEditorPanel::OnImGuiRender() {
    if (!m_IsOpen)
        return;

    ImGui::Begin(m_Name.c_str(), &m_IsOpen);

    for (const auto& file : pe::Project::Get().GetFiles()) {
        std::string uniqueName = file.GetFullName() + "##" + file.localPath.string();

        if (ImGui::Selectable(uniqueName.c_str())) {
            // noop, only for right clicks
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Load Object")) {
                LoadAsset(file);
            }
            if(ImGui::MenuItem("Load into assets")){

            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void AssetEditorPanel::LoadAsset(const pe::FileDescriptor& file){
    if(file.extension == ".obj"){
        pe::AssetSystem::Get().ImportAsset(&file);
    }
}
