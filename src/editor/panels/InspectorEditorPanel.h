#pragma once

#include "EditorPanel.h"
#include "IMessageHandler.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "ecs_common.h"

class ComponentEditor;

class InspectorEditorPanel : public EditorPanel, IMessageHandler{
public:
  InspectorEditorPanel();
  void HandleMessage(const Message& msg) override;

public:
  void OnImGuiRender() override;

private:
  void DrawComponents(ecs::Entity entity);
  template<typename T>
  void DrawComponentIfPresent(ecs::Entity entity);
  void DrawHeader(ecs::Entity entity);

private:
  Scene& r_Scene;

private:
  ecs::Entity m_SelectedEntity = ecs::NULL_ENTITY;
};
