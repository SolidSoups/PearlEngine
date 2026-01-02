#pragma once

#include "EditorPanel.h"
#include "Scene.h"
#include "MaterialData.h"

class SceneHierarchyEditorPanel : public EditorPanel{
public:
  SceneHierarchyEditorPanel();

public:
  void OnImGuiRender() override;

private:
  void DrawSceneHierarchy();  
  void DrawContextMenu();
};
