#pragma once

#include "CameraComponetnt.h"
#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"

class CameraComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(Component* target) override;
};

STATIC_REGISTER_COMPONENT_EDITOR(CameraComponent, CameraComponentEditor)
