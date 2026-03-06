#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "TextureConfig.h"
#include "json_common.h"

struct TextureData {
  uint32_t width;
  uint32_t height;
  uint32_t channels;
  std::string filePath;
  TextureConfig config{};
  // anything could set this id
  unsigned int id = 0;
  unsigned int GetTextureID() const { return id; }

public:
  explicit TextureData(){}
  ~TextureData();

  // allow move constructor and assignment
  TextureData(TextureData && other);
  TextureData &operator=(TextureData && other);

  // delete copy construct and assignment
  TextureData(TextureData&) = delete;
  TextureData &operator=(TextureData&) = delete;

  // set texture configuration for this texture
  void setConfig(const TextureConfig &config);
  // load an image file as the data for this texture
  bool loadFile(const char* path);
  // load raw image data for this texture
  bool loadData(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels);

public:
  void bind(unsigned int slot);
  static void unbind();

public:
  size_t getMemorySize() const { return width * height * channels; }
};

// JSON Serialization

inline void to_json(json& j, const TextureData& data){
  j["filePath"] = data.filePath;
  j["config"] = data.config;
}
// only usefule if i want to construct a Texture outside of the TextureCache
// inline void from_json(const json&j, TextureData& data){
//   TextureConfig myConfig = j["config"];
//   std::string myFilePath = j["filePath"];
//   data.setConfig(myConfig);     
//   data.loadFile(myFilePath.c_str());
// }
