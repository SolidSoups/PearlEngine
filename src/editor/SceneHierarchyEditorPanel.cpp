#include "SceneHierarchyEditorPanel.h"

#include "RenderComponent.h"
#include "imgui.h"
#include "MeshData.h"

void SceneHierarchyEditorPanel::OnImGuiRender(){
  if(!m_IsOpen) return;

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);

  const auto& sceneObjects = m_Scene.GetGameObjects();
  for(const auto& gameObject : sceneObjects){
    auto renderComp = gameObject->GetComponent<RenderComponent>();
    // Display each gameObject as a selectable
    std::string label = "GameObject " + std::to_string(gameObject->GetID());
    if(renderComp->materialHandle == m_SunnyHandle){
      label += " (Sunshine)";
    }
    else{
      label += " (Pearl)";
    }

    if(ImGui::Selectable(label.c_str())){
      if(renderComp->materialHandle == m_SunnyHandle)
        renderComp->materialHandle = m_PearlHandle;
      else
        renderComp->materialHandle = m_SunnyHandle;
    }
  }

  ImGui::End();
}
