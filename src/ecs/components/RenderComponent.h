#pragma once

#include "Component.h"
#include "MaterialData.h"
#include "Mesh.h"

// thats it
struct RenderComponent : Component{
  COMPONENT_CLASS(RenderComponent)

public:
  MeshHandle meshHandle;          // what to render
  MaterialHandle materialHandle;  // how to render it

  RenderComponent(
    MeshHandle meshH,
    MaterialHandle matH
  ) : meshHandle(meshH), materialHandle(matH) {}
};
