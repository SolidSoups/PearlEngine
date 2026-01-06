#pragma once

#include "EditorPanel.h"
#include "GameObject.h"
#include "IMessageHandler.h"
#include "Scene.h"
#include "ServiceLocator.h"

class InspectorEditorPanel : public EditorPanel, IMessageHandler{
public:
  InspectorEditorPanel(ServiceLocator* locator);
  void HandleMessage(const Message& msg) override;

public:
  void OnImGuiRender() override;

private:
  void DrawComponents(GameObject* go);
  void DrawHeader(GameObject* go);

private:
  Scene& r_Scene;

private:
  GameObject* r_selectedGameObject;
};
