#include "ProjectEditorPanel.h"

#include <filesystem>

#include "MeshData.h"
#include "RenderComponentEditor.h"
#include "TransformComponentEditor.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"

#include "Project.h"
#include "ResourceSystem.h"
#include "meshLoaders.h"
#include "Assets.h"

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

void ProjectEditorPanel::LoadAsset(const pe::File& file){
    if(file.extension == ".obj"){
        auto mesh = pe::Assets::Get().Load<Mesh>(&file);
        if(mesh){
            auto handle = ResourceSystem::Get().Create(std::move(*mesh));
            CreateSceneObjectWithMesh(handle, file.stem);
        }
    }
}

// IT SHOULD LOAD INTO THE ASSET REGISTRY, NOT THE GAME
void ProjectEditorPanel::CreateSceneObjectWithMesh(MeshHandle handle, const std::string& name) {
    // create mesh
    auto *go = m_Scene.CreateGameObject(name);
    go->AddComponent<RenderComponent>(handle, m_MatHandle);
    go->AddComponent<TransformComponent>();
}
