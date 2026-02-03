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

void RenderComponentEditor::OnDrawComponent(void *target, [[maybe_unused]] ecs::Entity entity) {
  RenderComponent *renderComp = static_cast<RenderComponent *>(target);
  if (!renderComp)
    return;

  bool openMeshPopup = false, openMatPopup = false;
  float labelWidth = 180.f;

  // display and update albedo map
  m_AlbedoTexture.renderImGui("texture_diffuse1");
  if(m_AlbedoTexture.isDirty()){
    if(auto newTexture = m_AlbedoTexture.create())
      renderComp->material->setTexture("texture_diffuse1", newTexture);
  }

  // display and update specular map
  m_SpecTexture.renderImGui("texture_specular1");
  if(m_SpecTexture.isDirty()){
    if(auto newTexture = m_SpecTexture.create())
      renderComp->material->setTexture("texture_specular1", newTexture);
  }

  // display and update normal map
  m_NormalTexture.renderImGui("texture_normal1");
  if(m_NormalTexture.isDirty()){
    if(auto newTexture = m_NormalTexture.create())
      renderComp->material->setTexture("texture_normal1", newTexture);
  }

  // display and update mesh
  if(ImGui::CollapsingHeader("Mesh")){
    DrawMesh(renderComp);
  }
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
