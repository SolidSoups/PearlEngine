#pragma once

#include <optional>

#include "File.h"
#include "Handle.h"

class IAssetLoader {
  public:
    virtual ~IAssetLoader() = default;
};

template <typename AssetType>
class AssetLoader : public IAssetLoader{
  public:
    virtual ~AssetLoader() = default;
    virtual std::optional<AssetType> LoadAsset(const pe::FileDescriptor *file) = 0;
};
