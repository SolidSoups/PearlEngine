#pragma once

#include "EditorPanel.h"
#include "GameObject.h"
#include "Scene.h"
#include "MaterialData.h"

class InspectorEditorPanel : public EditorPanel{
public:
  InspectorEditorPanel(Scene& scene);

public:
  void OnImGuiRender() override;

private:
  Scene& m_Scene;

  void DrawTransform(GameObject* go);
};
