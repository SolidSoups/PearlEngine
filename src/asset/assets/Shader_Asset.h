#pragma once

#include "IAsset.h"

struct Shader_Asset : public IAsset{
    ASSET_CLASS(Shader_Asset)
public:
public:
    void AcceptSerializer(ISerializationWriter* visitor) override{
    }
    void AcceptDeserializer(ISerializationReader* visitor) override{
    }
};
