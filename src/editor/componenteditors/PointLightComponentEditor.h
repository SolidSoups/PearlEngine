#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "PointLightComponent.h"

class PointLightComponentEditor : public ComponentEditor {
public:
  void OnDrawComponent(void* component, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "PointLightComponent"; }
};

STATIC_REGISTER_COMPONENT_EDITOR(PointLightComponent, PointLightComponentEditor)
