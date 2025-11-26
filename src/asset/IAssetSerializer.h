#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "IAsset.h"

class IAssetSerializer {
public:
    virtual ~IAssetSerializer() = default;
    virtual const std::vector<uint8_t> Serialize(IAsset* asset) = 0;
    virtual std::unique_ptr<IAsset> Deserialize(const std::vector<uint8_t>& data) = 0;
};
