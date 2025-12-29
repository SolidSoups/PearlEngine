#pragma once

#include "IAsset.h"

struct Material_Asset : public IAsset {
    ASSET_CLASS(Material_Asset)
public:

public:
    void AcceptSerializer(ISerializationWriter* visitor) override{
        // noop
    }
    void AcceptDeserializer(ISerializationReader* visitor) override{
        // noop
    }
};
