#pragma once

#include "CameraComponent.h"
#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"

class CameraComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(void* target, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "CameraComponent"; }
};

STATIC_REGISTER_COMPONENT_EDITOR(CameraComponent, CameraComponentEditor)
