#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "RigidBodyComponent.h"


class RigidBodyComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(void* target, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "Rigid Body Component"; }
};

STATIC_REGISTER_COMPONENT_EDITOR(RigidBodyComponent, RigidBodyComponentEditor)
