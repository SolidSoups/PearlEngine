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
};
