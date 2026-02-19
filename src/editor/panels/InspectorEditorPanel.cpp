#include "InspectorEditorPanel.h"
#include "ComponentEditor.h"
#include "Logger.h"
#include "MenuRegistry.h"

#include "ImGuiHelpers.h"
#include "Material.h"
#include "ComponentEditorRegistry.h"
#include "PointLightComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "NameComponent.h"
#include "SelectionWizard.h"
#include "RenderComponent.h"
#include "TransformComponentEditor.h"
#include "ScriptComponent.h"
#include "imgui.h"

#include <iostream>

InspectorEditorPanel::InspectorEditorPanel()
    : EditorPanel("Inspector"), r_Scene(ServiceLocator::Get<Scene>()) {
  MenuRegistry::Get().Register("Windows/Inspector", &m_IsOpen);
}

void InspectorEditorPanel::OnImGuiRender() {
  if (!m_IsOpen)
    return;

  ImGui::Begin(m_Name.c_str());

  ecs::Entity selectedEntity = SelectionWizard::Get();
  if (selectedEntity != ecs::NULL_ENTITY) {
    DrawHeader(selectedEntity);
    DrawComponents(selectedEntity);
  }

  ImGui::End();
}

void InspectorEditorPanel::DrawHeader(ecs::Entity entity) {
  static char nameBuffer[512];
  std::string entityName = r_Scene.GetEntityName(entity);
  strncpy(nameBuffer, entityName.c_str(), sizeof(nameBuffer) - 1);
  nameBuffer[sizeof(nameBuffer) - 1] = '\0';

  ImGui::BeginChild("GameObjectHeader", ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoScrollbar);
  if (ImGui::InputText("##GameObjectName", nameBuffer, sizeof(nameBuffer),
                       ImGuiInputTextFlags_EnterReturnsTrue)) {
    r_Scene.SetEntityName(entity, nameBuffer);
  }
  ImGui::EndChild();
}

template<typename T>
void InspectorEditorPanel::DrawComponentIfPresent(ecs::Entity entity) {
  auto& coordinator = r_Scene.GetCoordinator();
  if (!coordinator.HasComponent<T>(entity)) return;

  ComponentEditor* editor = GET_COMPONENT_EDITOR(T);
  const char* name = editor ? editor->GetComponentName() : typeid(T).name();

  if (ImGui::CollapsingHeader(name, ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));
    std::string contentName = std::string(name) + "Content";
    ImGui::BeginChild(contentName.c_str(), ImVec2(0, 0),
                      ImGuiChildFlags_AutoResizeY,
                      ImGuiWindowFlags_NoScrollbar);

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Indent(10.f);

    // here we draw the components editor
    if (editor) {
      T& comp = coordinator.GetComponent<T>(entity);
      editor->OnDrawComponent(&comp, entity);
    } else {
      ImGui::PushTextWrapPos(0.0f);
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.7f, 0.2f, 1.0f));
      ImGui::Text("WARNING: No component editor has been registered for %s "
                  "with the Component Editor Registry.",
                  name);
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

void InspectorEditorPanel::DrawComponents(ecs::Entity entity) {
  auto& coordinator = r_Scene.GetCoordinator();

  // Draw components in order (TransformComponent first as highest priority)
  DrawComponentIfPresent<TransformComponent>(entity);
  DrawComponentIfPresent<CameraComponent>(entity);
  DrawComponentIfPresent<RenderComponent>(entity);
  DrawComponentIfPresent<PointLightComponent>(entity);
  DrawComponentIfPresent<ScriptComponent>(entity);

  // draw add component
  ImGui::Separator();
  if (ImGui::Button("Add Component##Button", ImVec2(-1, 0))) {
    ImGui::OpenPopup("##SearchablePopup_Add_Component");
  }

  std::vector<std::string> compChoices = {"Render Component",
                                          "Transform", "Point Light", "Camera", "Script"};
  std::string selected = "";
  if (SearchablePopup<std::string>(
          "Add_Component", "Add Component", compChoices,
          [](std::string choice) { return choice; }, selected)) {
    if (selected == "Render Component" && !coordinator.HasComponent<RenderComponent>(entity)) {
      coordinator.AddComponent(entity, RenderComponent{});
    } else if (selected == "Transform" && !coordinator.HasComponent<TransformComponent>(entity)){
      coordinator.AddComponent(entity, TransformComponent{});
    } else if (selected == "Point Light" && !coordinator.HasComponent<PointLightComponent>(entity)){
      coordinator.AddComponent(entity, PointLightComponent{});
    } else if (selected == "Camera" && !coordinator.HasComponent<CameraComponent>(entity)){
      coordinator.AddComponent(entity, CameraComponent{});
    } else if(selected == "Script" && !coordinator.HasComponent<ScriptComponent>(entity)){
      coordinator.AddComponent(entity, ScriptComponent{});
      LOG_INFO << "entity has component script: " << (coordinator.HasComponent<ScriptComponent>(entity) ? "True" : "False");
    }
  }
}
