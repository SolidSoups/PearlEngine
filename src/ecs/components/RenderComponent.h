#pragma once

#include "Component.h"
#include "MaterialData.h"
#include "Mesh.h"
#include "AssetDescriptor.h"

// thats it
struct RenderComponent : Component{
  COMPONENT_CLASS(RenderComponent)

public:
  MeshHandle meshHandle;          // what to render
  MaterialHandle materialHandle;  // how to render it
  
  void SetMeshFromAsset(const pe::AssetDescriptor& desc);
  void SetMaterialFromTextureAsset(const pe::AssetDescriptor& desc);

  RenderComponent(
    MeshHandle meshH,
    MaterialHandle matH
  ) : meshHandle(meshH), materialHandle(matH) {}
};
