#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "BoxColliderComponent.h"

class BoxColliderComponentEditor : public ComponentEditor {
public:
  void OnDrawComponent(void* target, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "Box Collider Component"; }
};

STATIC_REGISTER_COMPONENT_EDITOR(BoxColliderComponent, BoxColliderComponentEditor)
