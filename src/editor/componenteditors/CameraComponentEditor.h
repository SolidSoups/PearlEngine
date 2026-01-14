#pragma once

#include "CameraComponent.h"
#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"

class CameraComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(IComponent* target) override;
};

STATIC_REGISTER_COMPONENT_EDITOR(CameraComponent, CameraComponentEditor)
