#pragma once

#include <cstddef>
#include <glad/glad.h>
#include <cstdint>
#include <string>

#include "Handle.h"
#include "ResourceMacros.h"
#include "Texture_Asset.h"

struct TextureData {
  RESOURCE_CLASS(TextureData)
public:
  TextureData(const std::vector<unsigned char>& pixelData, uint32_t width, uint32_t height, uint32_t channels, bool generateMipMaps);
  // deprecated, but keep for now
  TextureData(uint32_t _width, uint32_t _height, uint32_t _channels)
        : width(_width), height(_height), channels(_channels) {}
  TextureData(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels, bool genMipMaps=true);
  uint32_t width;
  uint32_t height;
  uint32_t channels;
  GLuint id = 0;
};
PEARL_DEFINE_RESOURCE(TextureData)

class ResourceSystem;

void BindTexture(ResourceSystem *rs, TextureDataHandle handle, unsigned int slot);
void UnbindTexture();
void DestroyTexture(ResourceSystem *rs, TextureDataHandle handle);
