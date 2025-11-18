#include "InspectorEditorPanel.h"
#include "MenuRegistry.h"

#include "ComponentEditorRegistry.h"
#include "TransformComponent.h"
#include "imgui.h"

#include <iostream>

InspectorEditorPanel::InspectorEditorPanel(Scene &scene)
    : EditorPanel("Inspector"), m_Scene(scene) {
  MenuRegistry::Get().Register("Windows/Inspector", &m_IsOpen);
}

void InspectorEditorPanel::OnImGuiRender() {
  if (!m_IsOpen)
    return;

  ImGui::Begin(m_Name.c_str());

  GameObject *selectedGO = m_Scene.GetSelectedGameObject();
  if (selectedGO) {
    DrawHeader(selectedGO);
    DrawComponents(selectedGO);
  }

  ImGui::End();
}

void InspectorEditorPanel::DrawHeader(GameObject *go) {
  static char nameBuffer[512];
  strncpy(nameBuffer, go->m_Name.c_str(), sizeof(nameBuffer) - 1);
  nameBuffer[sizeof(nameBuffer) - 1] = '\0';


  ImGui::BeginChild("GameObjectHeader", ImVec2(0,0), ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoScrollbar);
  if(ImGui::InputText("##GameObjectName", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue)){
    go->m_Name = nameBuffer;
  }
  ImGui::EndChild();
}

void InspectorEditorPanel::DrawComponents(GameObject *go) {
  const auto &components = go->GetAllComponents();

  for (auto &[key, comp] : components) {
    ComponentEditor *editor = GET_COMPONENT_EDITOR_WITH_TYPEID(comp->GetTypeIndex());
    const char *name = comp->GetComponentName();

    if (ImGui::CollapsingHeader(name, ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));
      std::string contentName = std::string(name) + "Content";
      ImGui::BeginChild(contentName.c_str(), ImVec2(0, 0), ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoScrollbar);

      ImGui::Dummy(ImVec2(0, 10));
      ImGui::Indent(10.f);

      // here we draw the components editor
      if (editor)
        editor->OnDrawComponent(comp.get());
      else {
        ImGui::PushTextWrapPos(0.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.7f, 0.2f, 1.0f));
        ImGui::Text("WARNING: No component editor has been registered for %s with the Component Editor Registry.", name);
        ImGui::PopStyleColor();
        ImGui::PopTextWrapPos();
      }

      ImGui::Unindent(10.0f);
      ImGui::Dummy(ImVec2(0, 10));
      ImGui::EndChild();

      ImGui::PopStyleVar();
      ImGui::PopStyleColor();
    }
  }
}
