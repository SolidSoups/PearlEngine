#pragma once

#include "ComponentEditor.h"
#include "ComponentEditorRegistry.h"
#include "TextureCreator.h"
#include "TerrainComponent.h"

class TerrainSystem;

class TerrainComponentEditor : public ComponentEditor {
public:
  void OnInit() override;
  void OnDrawComponent(void* target, ecs::Entity entity) override;
  const char* GetComponentName() const override { return "Terrain Component"; }

private:
  TextureCreator myHeightMap;
  TerrainSystem* myTerrainSystem;
};

STATIC_REGISTER_COMPONENT_EDITOR(TerrainComponent, TerrainComponentEditor)
