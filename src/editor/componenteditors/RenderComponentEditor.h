#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "TextureData.h"
#include "RenderComponent.h"


class RenderComponentEditor : public ComponentEditor{
public:
  void OnDrawComponent(Component* target) override;

private:
  void DrawTexture(RenderComponent* comp);
  void DrawMesh(RenderComponent* comp); 

  void trySetCompTexture(RenderComponent* comp, const std::string& slot, std::shared_ptr<TextureData> texture);
};

STATIC_REGISTER_COMPONENT_EDITOR(RenderComponent, RenderComponentEditor)
