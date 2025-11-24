#include "AssetEditorPanel.h"

#include <filesystem>

#include "MeshData.h"
#include "RenderComponentEditor.h"
#include "TransformComponentEditor.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"

#include "meshLoaders.h"


void AssetEditorPanel::OnImGuiRender(){
  if(!m_IsOpen)
    return;

  RefreshAssetPaths();

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);


  for(const auto& path : m_Files){
    if(ImGui::Selectable(path.c_str())){
      // noop, only for right clicks
    }

    if(ImGui::BeginPopupContextItem()){
      if(ImGui::MenuItem("Load Obj")){
        CreateSceneObjectFromFile(path);
      }

      ImGui::EndPopup();
    }
  }

  ImGui::End();
}

std::vector<std::string> getFilesRecursively(const std::string& directory){
  std::vector<std::string> files;
  for(const auto& entry : std::filesystem::recursive_directory_iterator(directory)){
    if(entry.is_regular_file() && entry.path().extension() == ".obj"){
      files.push_back(entry.path().string());
    }
  }
  return files;
}

void AssetEditorPanel::RefreshAssetPaths(){
  m_Files = getFilesRecursively(c_AssetDirectory); 
}

void AssetEditorPanel::CreateSceneObjectFromFile(const std::string filepath){
  // create mesh
  MeshOldHandle meshHandle = CreateMeshFromObjFile(filepath.c_str());

  auto* go = m_Scene.CreateGameObject();
  go->AddComponent<RenderComponent>(meshHandle, m_MatHandle);
  go->AddComponent<TransformComponent>();
}
