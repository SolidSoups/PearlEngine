#pragma once

#include "EditorPanel.h"
#include "Scene.h"
#include "ServiceHandle.h"
#include "ServiceLocator.h"
#include "SelectionWizard.h"

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
  ServiceHandle<SelectionWizard> r_SelectionWizard;
};
