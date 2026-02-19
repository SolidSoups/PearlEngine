#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "ScriptComponent.h"

class ScriptComponentEditor : public ComponentEditor {
public:
  void OnDrawComponent(void* component, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "ScriptComponent"; }
};

STATIC_REGISTER_COMPONENT_EDITOR(ScriptComponent, ScriptComponentEditor)
