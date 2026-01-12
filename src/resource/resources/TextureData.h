#pragma once

#include <cstddef>
#include <glad/glad.h>
#include <cstdint>
#include <string>
#include <vector>

struct TextureData {
  // Prevent copying
  TextureData(const TextureData&) = delete;
  TextureData& operator=(const TextureData&) = delete;

public:
  TextureData(const std::vector<unsigned char>& pixelData, uint32_t width, uint32_t height, uint32_t channels, bool generateMipMaps);
  // deprecated, but keep for now
  TextureData(uint32_t _width, uint32_t _height, uint32_t _channels)
        : width(_width), height(_height), channels(_channels) {}
  TextureData(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels, bool genMipMaps=true);

  // New method-based API
  void bind(unsigned int slot);
  static void unbind();

  uint32_t width;
  uint32_t height;
  uint32_t channels;
  GLuint id = 0;
};
