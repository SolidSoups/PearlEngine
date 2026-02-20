#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef unsigned int GLenum;
#define GL_LINEAR 0x2601
#define GL_REPEAT 0x2901

struct TextureConfig {
  bool generateMipMaps = true;
  uint8_t maxMipMapLevel = 4;
  GLenum minFilter = GL_LINEAR;
  GLenum magFilter = GL_LINEAR;
  float lodBias = 0.0f;
  float minLod = 0.0f;
  float maxLod = 1000.0f;
  int antisotropicLevel = 1;
  GLenum wrapS = GL_REPEAT;
  GLenum wrapT = GL_REPEAT;
  TextureConfig() = default;

  bool operator==(const TextureConfig& other) const {
    return (
      generateMipMaps == other.generateMipMaps &&
      maxMipMapLevel == other.maxMipMapLevel &&
      minFilter == other.minFilter &&
      magFilter == other.magFilter &&
      lodBias == other.lodBias &&
      minLod == other.minLod &&
      maxLod == other.maxLod &&
      antisotropicLevel == other.antisotropicLevel &&
      wrapS == other.wrapS &&
      wrapT == other.wrapT
    );
  }
  bool operator !=(const TextureConfig& other) const {
    return !(*this == other);
  }
};

inline void to_json(json &j, const TextureConfig& c){
	j["generateMipMaps"] = c.generateMipMaps;
	j["maxMipMapLevel"] = c.maxMipMapLevel;
	j["minFilter"] = c.minFilter;
	j["magFilter"] = c.magFilter;
	j["lodBias"] = c.lodBias;
	j["minLod"] = c.minLod;
	j["maxLod"] = c.maxLod;
	j["antisotropicLevel"] = c.antisotropicLevel;
	j["wrapS"] = c.wrapS;
	j["wrapT"] = c.wrapT;
}
inline void from_json(const json &j, TextureConfig& c){
	c.generateMipMaps = j["generateMipMaps"];
	c.maxMipMapLevel = j["maxMipMapLevel"];
	c.minFilter = j["minFilter"];
	c.magFilter = j["magFilter"];
	c.lodBias = j["lodBias"];
	c.minLod = j["minLod"];
	c.maxLod = j["maxLod"];
	c.antisotropicLevel = j["antisotropicLevel"];
	c.wrapS = j["wrapS"];
	c.wrapT = j["wrapT"];
}
