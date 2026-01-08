#include "ProjectEditorPanel.h"

#include <filesystem>

#include "imgui.h"

#include "Project.h"
#include "AssetSystem.h"

void ProjectEditorPanel::OnImGuiRender() {
    if (!m_IsOpen)
        return;

    ImGui::Begin(m_Name.c_str(), &m_IsOpen);

    for (const auto& file : pe::Project::Get().GetFiles()) {
        std::string uniqueName = file.GetFullName() + "##" + file.localPath.string();

        if (ImGui::Selectable(uniqueName.c_str())) {
            // noop, only for right clicks
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Import file to assets")) {
                LoadAsset(file);
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void ProjectEditorPanel::LoadAsset(const pe::FileDescriptor& file){
    if(file.extension == ".obj"){
        r_AssetSystem->CreateAsset(&file);
    }
}
