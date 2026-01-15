#include "InspectorEditorPanel.h"
#include "ComponentEditor.h"
#include "ComponentFlags.h"
#include "Logger.h"
#include "MenuRegistry.h"

#include "ImGuiHelpers.h"
#include "Material.h"
#include "ComponentEditorRegistry.h"
#include "MessageBus.h"
#include "PointLightComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "SelectionWizard.h"
#include "RenderComponent.h"
#include "TransformComponentEditor.h"
#include "imgui.h"

#include <iostream>

InspectorEditorPanel::InspectorEditorPanel()
    : EditorPanel("Inspector"), r_Scene(ServiceLocator::Get<Scene>()),
      r_selectedGameObject(nullptr) {
  MenuRegistry::Get().Register("Windows/Inspector", &m_IsOpen);
  ServiceLocator::Get<MessageBus>().Subscribe<SelectionMessage>(this);
}

void InspectorEditorPanel::HandleMessage(const Message &msg) {
  if (auto *evt = msg.As<SelectionMessage>()) {
    if (evt->type == Selection_Clear) {
      r_selectedGameObject = nullptr;
    } else if (evt->type == Selection_GameObject) {
      r_selectedGameObject = evt->SelectionAs<GameObject>();
    }
  }
}

void InspectorEditorPanel::OnImGuiRender() {
  if (!m_IsOpen)
    return;

  ImGui::Begin(m_Name.c_str());

  if (r_selectedGameObject) {
    DrawHeader(r_selectedGameObject);
    DrawComponents(r_selectedGameObject);
  }

  ImGui::End();
}

void InspectorEditorPanel::DrawHeader(GameObject *go) {
  static char nameBuffer[512];
  strncpy(nameBuffer, go->m_Name.c_str(), sizeof(nameBuffer) - 1);
  nameBuffer[sizeof(nameBuffer) - 1] = '\0';

  ImGui::BeginChild("GameObjectHeader", ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoScrollbar);
  if (ImGui::InputText("##GameObjectName", nameBuffer, sizeof(nameBuffer),
                       ImGuiInputTextFlags_EnterReturnsTrue)) {
    go->m_Name = nameBuffer;
  }
  ImGui::EndChild();
}

void InspectorEditorPanel::DrawComponents(GameObject *go) {
  const auto &components = go->GetAllComponents();

  // draw all highest priority components
  for(auto &[key, comp] : components){
    ComponentFlags flags = comp->GetCompFlags();
    if((ComponentFlags_SortHighest & flags) == 0) continue;

    DrawComp(comp.get());
  }

  // draw all other components
  for (auto &[key, comp] : components) {
    ComponentFlags flags = comp->GetCompFlags();
    if((ComponentFlags_SortHighest & flags) != 0) continue;

    DrawComp(comp.get());
  }

  // draw add component
  ImGui::Separator();
  if (ImGui::Button("Add Component##Button", ImVec2(-1, 0))) {
    ImGui::OpenPopup("##SearchablePopup_Add_Component");
  }

  std::vector<std::string> compChoices = {"Render Component", "Ambient Light",
                                          "Transform", "Point Light"};
  std::string selected = "";
  if (SearchablePopup<std::string>(
          "Add_Component", "Add Component", compChoices,
          [](std::string choice) { return choice; }, selected)) {
    if (selected == "Render Component") {
      go->AddComponent<RenderComponent>();
    } else if (selected == "Transform"){
      go->AddComponent<TransformComponent>();
    } else if (selected == "Point Light"){
      go->AddComponent<PointLightComponent>();
    }
  }
}

void InspectorEditorPanel::DrawComp(IComponent *comp) {
  if(!comp) return;

  ComponentEditor *editor =
    GET_COMPONENT_EDITOR_WITH_TYPEID(comp->GetTypeIndex());
  const char *name = comp->GetComponentName();

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
    if (editor)
      editor->OnDrawComponent(comp);
    else {
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
