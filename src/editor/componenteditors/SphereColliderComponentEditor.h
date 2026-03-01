#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "SphereColliderComponent.h"

class SphereColliderComponentEditor : public ComponentEditor {
public:
  void OnDrawComponent(void* target, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "Sphere Collider Component"; }
};

STATIC_REGISTER_COMPONENT_EDITOR(SphereColliderComponent, SphereColliderComponentEditor)
