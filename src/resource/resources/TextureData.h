#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "TextureConfig.h"

struct TextureData {
  TextureData(unsigned char *data, uint32_t width, uint32_t height,
              uint32_t channels, const TextureConfig& config = TextureConfig(), const std::string& filepath ="");
  TextureData(const TextureData &) = delete;
  TextureData &operator=(const TextureData &) = delete;

  void bind(unsigned int slot);
  static void unbind();

  uint32_t width;
  uint32_t height;
  uint32_t channels;
  std::string mFilePath;
  TextureConfig mConfig;

  const std::string& getFilePath() const { return mFilePath; }
  const TextureConfig& getConfig() const { return mConfig; }

  size_t getMemorySize() const { return width * height * channels; }
  unsigned int id = 0;
};
