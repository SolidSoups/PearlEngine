#include "SceneHierarchyEditorPanel.h"

#include "MenuRegistry.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "Camera.h"
#include "imgui.h"
#include <string>
#include "SelectionWizard.h"

SceneHierarchyEditorPanel::SceneHierarchyEditorPanel()
    : EditorPanel("Scene Hierarchy"), r_Scene(ServiceLocator::Get<Scene>()),
      r_SelectionWizard() {
  MenuRegistry::Get().Register("Windows/Scene Hierarchy", &m_IsOpen);
}

void SceneHierarchyEditorPanel::OnImGuiRender() {
  if (!m_IsOpen)
    return;

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);

  DrawSceneHierarchy();
  DrawContextMenu();

  // deselect if clicking on empty scene
  if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0) &&
      !ImGui::IsAnyItemHovered()) {
    r_SelectionWizard->ClearSelection();
  }

  ImGui::End();
}

void SceneHierarchyEditorPanel::DrawSceneHierarchy() {
  if(r_SelectionWizard.GetState() == Ready)
    LOG_INFO << "r_SelectionWizard is ready!";
  const auto &sceneObjects = r_Scene.GetGameObjects();
  for (size_t i = 0; i < sceneObjects.size(); i++) {
    const auto &gameObject = sceneObjects[i];
    auto renderComp = gameObject->GetComponent<RenderComponent>();
    // Display each gameObject as a selectable
    bool isSelected(r_SelectionWizard->GetSelectedObject<GameObject>() == gameObject.get());

    std::string nameid = gameObject->m_Name + "##" + std::to_string(i);

    if (ImGui::Selectable(nameid.c_str(), isSelected)) {
      LOG_INFO << "Setting selection to gameobject";
      r_SelectionWizard->SetSelection(Selection_GameObject, gameObject.get());
    }

    // context menu per gameobject
    std::string popupid = "GameObject_Context##" + std::to_string(i);
    if (ImGui::BeginPopupContextItem(popupid.c_str())) {
      ImGui::Text("%s", gameObject->m_Name.c_str());
      ImGui::Separator();

      // Preview camera button (only if GameObject has CameraComponent)
      if (auto* camComp = gameObject->GetComponent<CameraComponent>()) {
        if (ImGui::MenuItem("Preview Camera")) {
          ServiceLocator::Get<Camera>().StartPreview(camComp);
        }
      }

      ImGui::EndPopup();
    }
  }
}

void SceneHierarchyEditorPanel::DrawContextMenu() {
  ImGuiPopupFlags flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
  if (ImGui::BeginPopupContextWindow("HierarchyContextMenu", flags)) {
    if (ImGui::MenuItem("Create empty GameObject")) {
      r_Scene.CreateGameObject();
    }
    ImGui::EndPopup();
  }
}
