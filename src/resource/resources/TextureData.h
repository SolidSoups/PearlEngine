#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <string>

#include "Handle.h"
#include "ResourceMacros.h"

struct TextureData{
  uint32_t width;
  uint32_t height;
  uint32_t channels;
  GLuint id = 0;
};
PEARL_DEFINE_RESOURCE(Texture, TextureData)

void BindTexture(TextureHandle handle, unsigned int slot);
void UnbindTexture();
void DestroyTexture(TextureHandle handle);

