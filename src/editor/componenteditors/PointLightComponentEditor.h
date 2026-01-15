#pragma once

#include "Component.h"
#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "PointLightComponent.h"

class PointLightComponentEditor : public ComponentEditor {
public:
  void OnDrawComponent(IComponent* component) override;
};

STATIC_REGISTER_COMPONENT_EDITOR(PointLightComponent, PointLightComponentEditor) \

