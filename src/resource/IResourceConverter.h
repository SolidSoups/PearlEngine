#pragma once

#include "IAsset.h"
#include "IResource.h"

class IResourceToAssetConverter {
  public:
    virtual ~IResourceToAssetConverter() = default;

    // TODO: SHOULD RETURN HANDLE TF
    virtual IResource *ConvertToResource(IAsset *asset) = 0;
};
