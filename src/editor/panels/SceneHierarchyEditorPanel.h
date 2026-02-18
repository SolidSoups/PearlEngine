#pragma once

#include "EditorPanel.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "ecs_common.h"

class SceneHierarchyEditorPanel : public EditorPanel{
public:
  SceneHierarchyEditorPanel();

public:
  void OnImGuiRender() override;

private:
  void DrawSceneHierarchy();
  void DrawContextMenu();

private:
  Scene& r_Scene;
};
