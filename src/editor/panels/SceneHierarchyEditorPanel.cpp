#include "SceneHierarchyEditorPanel.h"

#include "MenuRegistry.h"
#include "RenderComponent.h"
#include "imgui.h"
#include "MeshData.h"
#include <string>

SceneHierarchyEditorPanel::SceneHierarchyEditorPanel(Scene& scene, MaterialHandle pearlHandle, MaterialHandle sunnyHandle) 
  : EditorPanel("Scene Hierarchy")
    , m_Scene(scene)
    , m_PearlHandle(pearlHandle) 
    , m_SunnyHandle(sunnyHandle)
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
    m_Scene.SetSelectedIndex(-1);
  }

  ImGui::End();
}

void SceneHierarchyEditorPanel::DrawSceneHierarchy(){
  const auto& sceneObjects = m_Scene.GetGameObjects();
  for(size_t i = 0; i < sceneObjects.size(); i++){
    const auto& gameObject = sceneObjects[i];
    auto renderComp = gameObject->GetComponent<RenderComponent>();
    // Display each gameObject as a selectable
    bool isSelected (m_Scene.GetSelectedGameObject() == gameObject.get());

    std::string nameid = gameObject->m_Name + "##" + std::to_string(i);

    if(ImGui::Selectable(nameid.c_str(), isSelected)){
      // TODO: change this shit
      if(renderComp){
        if(renderComp->materialHandle == m_SunnyHandle)
          renderComp->materialHandle = m_PearlHandle;
        else
          renderComp->materialHandle = m_SunnyHandle;
      }
      m_Scene.SetSelectedIndex(i);
    }
  }
}

void SceneHierarchyEditorPanel::DrawContextMenu(){
  ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
  if(ImGui::BeginPopupContextWindow("HierarchyContextMenu", flags)){
    if(ImGui::MenuItem("Create empty GameObject")){
      m_Scene.CreateGameObject();
    }
    ImGui::EndPopup();
  }
}
