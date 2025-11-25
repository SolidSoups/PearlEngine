#include "ProjectEditorPanel.h"

#include <filesystem>

#include "MeshData.h"
#include "RenderComponentEditor.h"
#include "TransformComponentEditor.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"

#include "Project.h"
#include "meshLoaders.h"

void ProjectEditorPanel::OnImGuiRender() {
    if (!m_IsOpen)
        return;

    ImGui::Begin(m_Name.c_str(), &m_IsOpen);

    for (const auto &file : pe::Project::Get().GetFiles()) {
        std::string uniqueName = file.GetFullName() + "##" + file.localPath.string();

        if (ImGui::Selectable(uniqueName.c_str())) {
            // noop, only for right clicks
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Load Object")) {
                CreateSceneObjectFromFile(file.localPath);
            }
            if(ImGui::MenuItem("Load into assets")){

            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void ProjectEditorPanel::CreateSceneObjectFromFile(const std::string filepath) {
    // create mesh
    MeshHandle meshHandle = CreateMeshFromObjFile(filepath.c_str());

    auto *go = m_Scene.CreateGameObject();
    go->AddComponent<RenderComponent>(meshHandle, m_MatHandle);
    go->AddComponent<TransformComponent>();
}
