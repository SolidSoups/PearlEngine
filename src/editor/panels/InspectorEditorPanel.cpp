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


  ImGui::Begin(m_Name.c_str());

  GameObject* selectedGO = m_Scene.GetSelectedGameObject();
  if(selectedGO){
    DrawTransform(selectedGO);
  }

  ImGui::End();
}

void DrawDraggableFloat3(const std::string& name, float* value, float scale, float* minscale = nullptr){
  const float labelWidth = 80.f;
  ImGui::AlignTextToFramePadding();
  ImGui::Text(name.c_str());
  ImGui::SameLine(labelWidth);
  if(minscale){
    ImGui::DragFloat3(("##" + name).c_str(), value, scale, *minscale);
  }
  else{
    ImGui::DragFloat3(("##" + name).c_str(), value, scale);
  }
}

void InspectorEditorPanel::DrawTransform(GameObject* go){
  TransformComponent* transform = go->GetComponent<TransformComponent>();
  if(!transform) return;
  
  if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)){
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));

    float labelWidth = 80.f;

    DrawDraggableFloat3("Position", &transform->position.x, 0.1f);
    DrawDraggableFloat3("Rotation", &transform->rotation.x, 0.5f);

    float minscale = 0.001f;
    DrawDraggableFloat3("Scale", &transform->scale.x, 0.1f, &minscale);

    ImGui::PopStyleVar();
  }
}

