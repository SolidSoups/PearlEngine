#pragma once

#include "EditorPanel.h"
#include "GameObject.h"
#include "Scene.h"
#include "MaterialData.h"

class InspectorEditorPanel : public EditorPanel{
public:
  InspectorEditorPanel();

public:
  void OnImGuiRender() override;

private:
  void DrawComponents(GameObject* go);
  void DrawHeader(GameObject* go);
};
