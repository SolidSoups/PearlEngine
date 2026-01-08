#pragma once

#include "IAsset.h"
#include <algorithm>
#include <cstdint>
#include <vector>
#include "Logger.h"

struct Texture_Asset : public IAsset {
  ASSET_CLASS(Texture_Asset)
public:
  Texture_Asset() = default;
  Texture_Asset(std::vector<unsigned char> data, uint32_t width, uint32_t height, uint32_t channels)
    : pixelData(std::move(data)), width(width), height(height), channels(channels)
  {}
public:
  std::vector<unsigned char> pixelData;
  uint32_t width = 0, height = 0, channels = 0;

public:
  void AcceptSerializer(ISerializationWriter *visitor) override {
    visitor->WriteInt32("width", width);
    visitor->WriteInt32("height", height);
    visitor->WriteInt32("channels", channels);
    visitor->WriteData("pixelData", pixelData.data(), pixelData.size());
  }
  void AcceptDeserializer(ISerializationReader *visitor) override {
    visitor->ReadInt32("width", width);
    visitor->ReadInt32("height", height);
    visitor->ReadInt32("channels", channels);
    visitor->ReadData("pixelData", pixelData);
  }
};
