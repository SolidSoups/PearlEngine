#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "TransformComponent.h"

class TransformComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(void* target, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "TransformComponent"; }
  const int GetSortOrder() const override { return -100; } // Draw first
};

STATIC_REGISTER_COMPONENT_EDITOR(TransformComponent, TransformComponentEditor)
