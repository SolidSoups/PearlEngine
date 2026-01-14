#include "RenderComponentEditor.h"

#include "Defaults.h"
#include "TextureManager.h"
#include "UserGUI.h"
#include "imgui.h"

#include "FileSystem.h"
#include "MeshManager.h"
#include "MaterialLoader.h"
#include "ServiceLocator.h"
#include "ImGuiHelpers.h"

void RenderComponentEditor::OnDrawComponent(Component *target) {
  RenderComponent *renderComp = dynamic_cast<RenderComponent *>(target);
  if (!renderComp)
    return;

  bool openMeshPopup = false, openMatPopup = false;
  float labelWidth = 180.f;

  DrawTexture(renderComp);
  DrawMesh(renderComp);

  // draw mesh section
  auto meshFiles = FileSystem::queryFiles(".obj");
  auto texFiles = FileSystem::queryFiles(".png");
  FileSystem::FileDescriptor selectedFile;
}

void RenderComponentEditor::trySetCompTexture(
    RenderComponent *comp, const std::string &slot,
    std::shared_ptr<TextureData> texPtr) {
  // set loaded texture
  if (!comp->material.get())
    comp->material = MaterialLoader::create(
        Defaults::getDefaultShader());
  comp->material->setTexture(slot, texPtr);
}

void RenderComponentEditor::DrawTexture(RenderComponent *comp) {
  float labelWidth = 180.f;
  bool openMaterialSearch = false;

  // draw albedo texture section
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Albedo Texture:");
  ImGui::SameLine(labelWidth);
  std::string albedoInfo = !comp->material ? "nullptr"
                           : (comp->material->textureExists("albedoMap"))
                               ? "Valid"
                               : "None";
  ImGui::Text("%s", albedoInfo.c_str());
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  if (ImGui::Button("Set Texture...##setalbedo")) {
    // start texture dialog
    UserGUI::StartTexturePopup(
        [this, comp](std::shared_ptr<TextureData> texPtr) {
          trySetCompTexture(comp, "albedoMap", texPtr);
        });
  }

  //  draw albedo texture section
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Specular Texture:");
  ImGui::SameLine(labelWidth);
  std::string specInfo = !comp->material ? "nullptr"
                           : (comp->material->textureExists("specularMap"))
                               ? "Valid"
                               : "None";
  ImGui::Text("%s", specInfo.c_str());
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  if (ImGui::Button("Set Texture...##setspecular")) {
    // start texture dialog
    UserGUI::StartTexturePopup(
        [this, comp](std::shared_ptr<TextureData> texPtr) {
          trySetCompTexture(comp, "specularMap", texPtr);
        });
  }
}

void RenderComponentEditor::DrawMesh(RenderComponent *comp) {
  float labelWidth = 180.f;
  ImGui::Text("Mesh:");
  ImGui::SameLine(labelWidth);
  std::string meshInfo = comp->mesh ? "Valid" : "nullptr";
  ImGui::Text("%s", &meshInfo[0]);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  if (ImGui::Button("Set Mesh...")) {
    // start mesh dialog
    UserGUI::StartMeshPopup([comp](std::shared_ptr<Mesh> meshPtr) {
      // set loaded mesh
      comp->mesh = meshPtr;
    });
  }
}
