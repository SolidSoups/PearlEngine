#pragma once

#include <memory>

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "ScriptComponent.h"


struct TextureData;

class ScriptComponentEditor : public ComponentEditor {
public:
  void OnInit() override;
  void OnDrawComponent(void* component, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "ScriptComponent"; }

public:
  std::shared_ptr<TextureData> m_CheckTex;
  std::shared_ptr<TextureData> m_CrossTex;
};

STATIC_REGISTER_COMPONENT_EDITOR(ScriptComponent, ScriptComponentEditor)
