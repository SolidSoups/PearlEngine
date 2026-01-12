#include "RenderComponentEditor.h"

#include "DefaultResources.h"
#include "TextureLoader.h"
#include "imgui.h"

#include "FileSystem.h"
#include "MeshManager.h"
#include "MaterialLoader.h"
#include "ServiceLocator.h"
#include "ImGuiHelpers.h"

void RenderComponentEditor::OnDrawComponent(Component *target) {
  RenderComponent *renderComp = dynamic_cast<RenderComponent *>(target);
  if(!renderComp) return;

  bool openMeshPopup = false, openMatPopup = false;
  float labelWidth = 180.f;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Material:");
  ImGui::SameLine(labelWidth);
  std::string matInfo = renderComp->material ? "Valid" : "nullptr";
  ImGui::Text("%s", &matInfo[0]);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  if(ImGui::Button("Set Texture...")){
    ImGui::OpenPopup("##SearchablePopup_MatPopup");
    openMatPopup = true;
  }

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Mesh:");
  ImGui::SameLine(labelWidth);
  std::string meshInfo = renderComp->mesh ? "Valid" : "nullptr";
  ImGui::Text("%s", &meshInfo[0]);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  if(ImGui::Button("Set Mesh...")){
    ImGui::OpenPopup("##SearchablePopup_MeshPopup");
    openMeshPopup = true;
  }

  auto meshFiles = FileSystem::queryFiles(".obj");
  auto texFiles = FileSystem::queryFiles(".png");
  FileSystem::FileDescriptor selectedFile;

  // try load mesh
  if(SearchablePopup<FileSystem::FileDescriptor>("MeshPopup", "Select a mesh", meshFiles, 
                                                 [](FileSystem::FileDescriptor desc){ return desc.stem;}, 
                                                 selectedFile)){
    LOG_INFO << "Loading mesh";
    std::shared_ptr<Mesh> loadedMesh = ServiceLocator::Get<MeshManager>().loadOBJ(selectedFile.localPath.c_str());
    LOG_INFO << "Mesh is loaded";
    if(loadedMesh){
      LOG_INFO << "Set render comp mesh";
      renderComp->mesh = loadedMesh;
    }
  }


  // try load texture
  if(SearchablePopup<FileSystem::FileDescriptor>("MatPopup", "Select a texture", texFiles, 
                                                 [](FileSystem::FileDescriptor desc){ return desc.stem;}, 
                                                 selectedFile)){
    // try to load the texture
    auto texPtr = TextureLoader::load(selectedFile.localPath.c_str(), false);
    if(!texPtr){
      LOG_ERROR << "Texture could not be loaded!";
      return;
    } 

    if(renderComp->material.get()){
      LOG_INFO << "Material pointer exists!";
    }
    else{
      renderComp->material = MaterialLoader::create(ServiceLocator::Get<DefaultResources>().getDefaultShader());
      renderComp->material->setTexture("mainTexture", texPtr);
      LOG_INFO << "Created material and set texture";
    }
  }
}
