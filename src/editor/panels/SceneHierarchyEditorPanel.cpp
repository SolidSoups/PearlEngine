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

  // iterate through scene objects
  const auto &sceneObjects = r_Scene.GetGameObjects();
  for (size_t i = 0; i < sceneObjects.size(); i++) {
    const auto &gameObject = sceneObjects[i];

    // is this game object selected?
    GameObject* selection = r_SelectionWizard->GetSelectedObject<GameObject>();
    bool isSelected = selection == gameObject.get();

    // create name id
    std::string nameid = gameObject->m_Name + "##" + std::to_string(i);

    // show selectable for gameobject
    if (ImGui::Selectable(nameid.c_str(), isSelected)) {
      // if pressed, set selection to this gameobject
      r_SelectionWizard->SetSelection(Selection_GameObject, gameObject.get());
    }

    // context menu per gameobject
    std::string popupid = "GameObject_Context##" + std::to_string(i);
    if (ImGui::BeginPopupContextItem(popupid.c_str())) {
      // show name
      ImGui::Text("%s", gameObject->m_Name.c_str());
      ImGui::Separator();

      // Preview camera button (only if GameObject has CameraComponent)
      // TODO: this will get cluttered the more functionality i add to the 
      // scene hierarchy panel
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
  // allow pressing right mouse and dissalow opening over gameobject items
  ImGuiPopupFlags flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;

  if (ImGui::BeginPopupContextWindow("HierarchyContextMenu", flags)) {
    // create an empty gameObject
    if (ImGui::MenuItem("Create empty GameObject")) {
      r_Scene.CreateGameObject();
    }
    if(ImGui::MenuItem("Create PointLight")){
      r_Scene.CreatePointLight();
    }
    if(ImGui::MenuItem("Create Cube")){
      r_Scene.CreateCube();
    }
    ImGui::EndPopup();
  }
}
