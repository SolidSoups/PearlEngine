#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "TextureConfig.h"
#include "json_common.h"

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
  std::string filePath;
  TextureConfig config;

  const std::string& getFilePath() const { return filePath; }
  const TextureConfig& getConfig() const { return config; }

  size_t getMemorySize() const { return width * height * channels; }
  unsigned int id = 0;
};

inline void to_json(json& j, const TextureData& data){
  j["filePath"] = data.filePath;
  j["config"] = data.config;
}
