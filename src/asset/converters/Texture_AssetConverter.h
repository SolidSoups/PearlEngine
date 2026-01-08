#pragma once

#include "IAssetConverter.h"
#include "AssetSystem.h"

class Texture_AssetConverter : public IAssetConverter {
  bool LoadTexture(const char *filePath, uint32_t &outWidth, uint32_t& outHeight,
                   uint32_t& outChannels,
                   std::vector<unsigned char> &outPixelData);

public:
  std::unique_ptr<IAsset>
  ConvertToAsset(const pe::FileDescriptor *file) override;
  std::unique_ptr<IAsset> CreateEmptyAsset() override;
};
