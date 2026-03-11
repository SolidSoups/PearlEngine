#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "TextComponent.h"
#include "TextureCreator.h"

class TextComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(void* target, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "Text Component"; }

private:
  ecs::Entity myLastEntity = ecs::NULL_ENTITY;
};

STATIC_REGISTER_COMPONENT_EDITOR(TextComponent, TextComponentEditor)
