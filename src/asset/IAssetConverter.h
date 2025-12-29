#pragma once

#include <memory>

#include "IAsset.h"
#include "FileDescriptor.h"

class IAssetConverter{
public:
    virtual ~IAssetConverter() = default;   
    virtual std::unique_ptr<IAsset> ConvertToAsset(const pe::FileDescriptor* file) = 0;
    virtual std::unique_ptr<IAsset> CreateEmptyAsset() = 0;
};
