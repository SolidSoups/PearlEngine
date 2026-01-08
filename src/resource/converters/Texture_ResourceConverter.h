#pragma once

#include "IResourceConverter.h"

class Texture_ResourceConverter : public IResourceToAssetConverter{
public:
  IResource *ConvertToResource(IAsset *asset) override;
};
