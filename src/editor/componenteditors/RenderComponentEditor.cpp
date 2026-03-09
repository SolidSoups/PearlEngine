#include "RenderComponentEditor.h"

#include "Defaults.h"
#include "TextureManager.h"
#include "UserGUI.h"
#include "imgui.h"

#include "FileSystem.h"
#include "MessageQueue.h"
#include "CreateMeshMessage.h"
#include "MeshManager.h"
#include "MaterialLoader.h"
#include "ServiceLocator.h"
#include "ImGuiHelpers.h"

void RenderComponentEditor::OnDrawComponent(void *target, ecs::Entity entity) {
  RenderComponent *renderComp = static_cast<RenderComponent *>(target);
  if (!renderComp)
    return;

  if (entity != m_LastEntity) {
    m_LastEntity = entity;
    if (renderComp->material) {
      m_AlbedoTexture.syncFrom(renderComp->material->getTexture("texture_diffuse1"));
      m_SpecTexture.syncFrom(renderComp->material->getTexture("texture_specular1"));
      m_NormalTexture.syncFrom(renderComp->material->getTexture("texture_normal1"));
    } else {
      m_AlbedoTexture.reset();
      m_SpecTexture.reset();
      m_NormalTexture.reset();
    }
  }

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

  // display the tiling and offset stuff
  ImGui::Dummy(ImVec2(0, 5));
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Tiling");
  ImGui::DragFloat2("##Tiling", &renderComp->material->tiling.x, 0.01f);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Offset");
  ImGui::DragFloat2("##Offset", &renderComp->material->offset.x, 0.01f);
  ImGui::Dummy(ImVec2(0, 5));

  // display and update mesh
  if(ImGui::CollapsingHeader("Mesh")){
    DrawMesh(renderComp, entity);
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

void RenderComponentEditor::DrawMesh(RenderComponent *comp, ecs::Entity entity) {
  float labelWidth = 180.f;
  ImGui::Text("Mesh:");
  ImGui::SameLine(labelWidth);
  std::string meshInfo = comp->mesh ? "Valid" : "nullptr";
  ImGui::Text("%s", &meshInfo[0]);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  if (ImGui::Button("Set Mesh...")) {
    // start mesh dialog
    UserGUI::StartFilePopup([entity](const std::string& filePath){
      auto& msgQueue = ServiceLocator::Get<MessageQueue>();

      // create new "create mesh message"
      CreateMeshMessage msg;
      msg.filePath = filePath;
      msg.renderCompEntity = entity;
      msgQueue.Dispatch(msg);
    }, {".obj"});
  }
}
