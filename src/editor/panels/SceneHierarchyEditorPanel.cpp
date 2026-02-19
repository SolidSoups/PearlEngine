#include "SceneHierarchyEditorPanel.h"

#include "MenuRegistry.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "Camera.h"
#include "imgui.h"
#include <string>
#include "SelectionWizard.h"

SceneHierarchyEditorPanel::SceneHierarchyEditorPanel()
    : EditorPanel("Scene Hierarchy"), r_Scene(ServiceLocator::Get<Scene>()) {
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
    SelectionWizard::Clear();
  }

  ImGui::End();
}

void SceneHierarchyEditorPanel::DrawSceneHierarchy() {
  // iterate through scene entities
  const auto &entities = r_Scene.GetEntities();
  auto& coordinator = r_Scene.GetCoordinator();

  for (size_t i = 0; i < entities.size(); i++) {
    ecs::Entity entity = entities[i];

    // is this entity selected?
    ecs::Entity selectedEntity = SelectionWizard::Get();
    bool isSelected = selectedEntity == entity;

    // get entity name
    std::string entityName = r_Scene.GetEntityName(entity);

    // create name id
    std::string nameid = entityName + "##" + std::to_string(i);

    // show selectable for entity
    if (ImGui::Selectable(nameid.c_str(), isSelected)) {
      // if pressed, set selection to this entity
      SelectionWizard::Set(entity);
    }

    // context menu per entity
    std::string popupid = "Entity_Context##" + std::to_string(i);
    if (ImGui::BeginPopupContextItem(popupid.c_str())) {
      // show name
      ImGui::Text("%s", entityName.c_str());
      ImGui::Separator();

      if(ImGui::MenuItem("Delete")){
        r_Scene.DestroyEntity(entity);
      }

      // Preview camera button (only if Entity has CameraComponent)
      if (coordinator.HasComponent<CameraComponent>(entity)) {
        if (ImGui::MenuItem("Preview Camera")) {
          ServiceLocator::Get<Camera>().StartPreview(entity);
        }
      }

      ImGui::EndPopup();
    }
  }
}

void SceneHierarchyEditorPanel::DrawContextMenu() {
  // allow pressing right mouse and dissalow opening over entity items
  ImGuiPopupFlags flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;

  if (ImGui::BeginPopupContextWindow("HierarchyContextMenu", flags)) {
    // create an empty Entity
    if (ImGui::MenuItem("Create empty Entity")) {
      r_Scene.CreateEntity();
    }
    if(ImGui::MenuItem("Create PointLight")){
      r_Scene.CreatePointLight();
    }
    if(ImGui::BeginMenu("Primitives")){
      if(ImGui::MenuItem("Create Plane")) r_Scene.CreatePlane();
      if(ImGui::MenuItem("Create Cube"))  r_Scene.CreateCube();
      if(ImGui::MenuItem("Create Sphere")) r_Scene.CreateSphere();
      if(ImGui::MenuItem("Create Cylinder"));
      ImGui::EndMenu();
    }
    ImGui::EndPopup();
  }
}
