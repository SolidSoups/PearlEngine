#pragma once

#include "EditorPanel.h"
#include "GameObject.h"
#include "Scene.h"
#include "ServiceLocator.h"

class InspectorEditorPanel : public EditorPanel{
public:
  InspectorEditorPanel(ServiceLocator* locator);

public:
  void OnImGuiRender() override;

private:
  void DrawComponents(GameObject* go);
  void DrawHeader(GameObject* go);

private:
  Scene& r_Scene;
};
