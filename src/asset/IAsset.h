#pragma once

#include "ISerializationWriter.h"
#include "UUID.h"

struct IAsset{
public:
    virtual ~IAsset() = default;
    UUID uuid;

    virtual void AcceptSerializer(ISerializationWriter* visitor) = 0;
};
