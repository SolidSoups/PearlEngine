#pragma once

#include "EditorPanel.h"
#include "Scene.h"
#include "MaterialData.h"
#include "ServiceLocator.h"

class SceneHierarchyEditorPanel : public EditorPanel{
public:
  SceneHierarchyEditorPanel(ServiceLocator* locator);

public:
  void OnImGuiRender() override;

private:
  void DrawSceneHierarchy();  
  void DrawContextMenu();

private:
  Scene& r_Scene;
};
