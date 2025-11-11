#pragma once

#include "Component.h"
#include "MaterialData.h"
#include "MeshData.h"

// thats it
struct RenderComponent : Component{
  MeshHandle meshHandle;          // what to render
  MaterialHandle materialHandle;  // how to render it

  RenderComponent(
    MeshHandle meshH,
    MaterialHandle matH
  ) : meshHandle(meshH), materialHandle(matH) {}
};
