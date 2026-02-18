#pragma once

#include "EditorPanel.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "ecs_common.h"

class ComponentEditor;

class InspectorEditorPanel : public EditorPanel {
public:
  InspectorEditorPanel();

public:
  void OnImGuiRender() override;

private:
  void DrawComponents(ecs::Entity entity);
  template<typename T>
  void DrawComponentIfPresent(ecs::Entity entity);
  void DrawHeader(ecs::Entity entity);

private:
  Scene& r_Scene;
};
