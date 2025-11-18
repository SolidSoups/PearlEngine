#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "TransformComponent.h"

class TransformComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(Component* target) override;
};

STATIC_REGISTER_COMPONENT_EDITOR(TransformComponent, TransformComponentEditor)
