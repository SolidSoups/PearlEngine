#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "CapsuleColliderComponent.h"

class CapsuleColliderComponentEditor : public ComponentEditor {
public:
  void OnDrawComponent(void* target, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "Capsule Collider Component"; }
};

STATIC_REGISTER_COMPONENT_EDITOR(CapsuleColliderComponent, CapsuleColliderComponentEditor)
