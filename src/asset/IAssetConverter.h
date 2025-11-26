#pragma once

#include <memory>

#include "IAsset.h"
#include "File.h"

class IAssetConverter{
public:
    virtual ~IAssetConverter() = default;   
    virtual std::unique_ptr<IAsset> ConvertToAsset(const pe::FileDescriptor* file) = 0;
};
