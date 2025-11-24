#pragma once

#include "Component.h"
#include "MaterialData.h"
#include "MeshData.h"

// thats it
struct RenderComponent : Component{
  COMPONENT_CLASS(RenderComponent)

public:
  MeshOldHandle meshHandle;          // what to render
  MaterialHandle materialHandle;  // how to render it

  RenderComponent(
    MeshOldHandle meshH,
    MaterialHandle matH
  ) : meshHandle(meshH), materialHandle(matH) {}
};
