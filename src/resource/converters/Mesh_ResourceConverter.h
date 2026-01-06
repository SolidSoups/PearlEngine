#pragma once

#include "IResourceToAssetConverter.h"
#include "Logger.h"
#include "Mesh.h"
#include "Mesh_Asset.h"
#include "ResourceSystem.h"

class Mesh_ResourceConverter : public IResourceToAssetConverter {
  public:
    IResource *ConvertToResource(IAsset *asset) override;
};
