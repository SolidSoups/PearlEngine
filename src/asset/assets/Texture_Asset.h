#pragma once

#include "IAsset.h"

struct Texture_Asset : public IAsset {
    ASSET_CLASS(Texture_Asset)
  public:
  public:
    void AcceptSerializer(ISerializationWriter *visitor) override {
        // noop
    }
    void AcceptDeserializer(ISerializationReader *visitor) override {
        // noop
    }
};
