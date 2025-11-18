#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "RenderComponent.h"


class RenderComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(Component* target) override;
};

STATIC_REGISTER_COMPONENT_EDITOR(RenderComponent, RenderComponentEditor)
