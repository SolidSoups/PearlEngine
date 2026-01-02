#include "SceneHierarchyEditorPanel.h"

#include "MenuRegistry.h"
#include "RenderComponent.h"
#include "imgui.h"
#include <string>

SceneHierarchyEditorPanel::SceneHierarchyEditorPanel(ServiceLocator* engineLocator) 
  : EditorPanel("Scene Hierarchy"), r_Scene(engineLocator->Get<Scene>())
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
    r_Scene.SetSelectedIndex(-1);
  }

  ImGui::End();
}

void SceneHierarchyEditorPanel::DrawSceneHierarchy(){
  const auto& sceneObjects = r_Scene.GetGameObjects();
  for(size_t i = 0; i < sceneObjects.size(); i++){
    const auto& gameObject = sceneObjects[i];
    auto renderComp = gameObject->GetComponent<RenderComponent>();
    // Display each gameObject as a selectable
    bool isSelected (r_Scene.GetSelectedGameObject() == gameObject.get());

    std::string nameid = gameObject->m_Name + "##" + std::to_string(i);

    if(ImGui::Selectable(nameid.c_str(), isSelected)){
      r_Scene.SetSelectedIndex(i);
    }
  }
}

void SceneHierarchyEditorPanel::DrawContextMenu(){
  ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
  if(ImGui::BeginPopupContextWindow("HierarchyContextMenu", flags)){
    if(ImGui::MenuItem("Create empty GameObject")){
      r_Scene.CreateGameObject();
    }
    ImGui::EndPopup();
  }
}
