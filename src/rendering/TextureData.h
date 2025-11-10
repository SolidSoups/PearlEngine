#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <string>

#include "Handle.h"

struct TextureData{
  uint32_t width;
  uint32_t height;
  uint32_t channels;
  GLuint textureID = 0;
};
struct TextureTag {};
using TextureHandle = Handle<TextureTag>;

TextureData LoadTexture(const std::string& filepath, bool generateMipMaps = true);
void BindTexture(TextureData texData, unsigned int slot);
void UnbindTexture();

