#pragma once

#include <glad/glad.h>
#include <cstdint>

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
