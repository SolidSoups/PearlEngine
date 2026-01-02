#include "SceneHierarchyEditorPanel.h"

#include "EngineContext.h"
#include "MenuRegistry.h"
#include "RenderComponent.h"
#include "imgui.h"
#include <string>

SceneHierarchyEditorPanel::SceneHierarchyEditorPanel() 
  : EditorPanel("Scene Hierarchy")
  {
  MenuRegistry::Get().Register("Windows/Scene Hierarchy", &m_IsOpen);
}

void SceneHierarchyEditorPanel::OnImGuiRender(){
  if(!m_IsOpen) return;

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);

  DrawSceneHierarchy();

  DrawContextMenu();

  // deselect if clicking on empty scene
  if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered()){
    EngineContext::Instance->GetScene().SetSelectedIndex(-1);
  }

  ImGui::End();
}

void SceneHierarchyEditorPanel::DrawSceneHierarchy(){
  const auto& sceneObjects = EngineContext::Instance->GetScene().GetGameObjects();
  for(size_t i = 0; i < sceneObjects.size(); i++){
    const auto& gameObject = sceneObjects[i];
    auto renderComp = gameObject->GetComponent<RenderComponent>();
    // Display each gameObject as a selectable
    bool isSelected (EngineContext::Instance->GetScene().GetSelectedGameObject() == gameObject.get());

    std::string nameid = gameObject->m_Name + "##" + std::to_string(i);

    if(ImGui::Selectable(nameid.c_str(), isSelected)){
      // TODO: change this shit
      // if(renderComp){
      //   if(renderComp->materialHandle == m_SunnyHandle)
      //     renderComp->materialHandle = m_PearlHandle;
      //   else
      //     renderComp->materialHandle = m_SunnyHandle;
      // }
      EngineContext::Instance->GetScene().SetSelectedIndex(i);
    }
  }
}

void SceneHierarchyEditorPanel::DrawContextMenu(){
  ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
  if(ImGui::BeginPopupContextWindow("HierarchyContextMenu", flags)){
    if(ImGui::MenuItem("Create empty GameObject")){
      EngineContext::Instance->GetScene().CreateGameObject();
    }
    ImGui::EndPopup();
  }
}
