#include "InspectorEditorPanel.h"
#include "MenuRegistry.h"

#include "TransformComponent.h"
#include "imgui.h"

#include <iostream>


InspectorEditorPanel::InspectorEditorPanel(Scene& scene)
  : EditorPanel("Inspector"), m_Scene(scene)
{
  MenuRegistry::Get().Register("Windows/Inspector", &m_IsOpen);
}

void InspectorEditorPanel::OnImGuiRender(){
  if(!m_IsOpen) return;


  ImGui::Begin("Inspector");

  GameObject* selectedGO = m_Scene.GetSelectedGameObject();
  if(selectedGO){
    std::cout << "Drawing selected gameobject" << std::endl;
    DrawTransform(selectedGO);
  }

  ImGui::End();
}

void InspectorEditorPanel::DrawTransform(GameObject* go){
  TransformComponent* transform = go->GetComponent<TransformComponent>();
  if(!transform) return;
  
  if(ImGui::CollapsingHeader("Transform")){
    ImGui::DragFloat3("Position", &transform->position.x, 0.1f);
    ImGui::DragFloat3("Rotation", &transform->rotation.x, 0.5f);
    ImGui::DragFloat3("Scale", &transform->scale.x, 0.1f, 0.001f);
  }
}
