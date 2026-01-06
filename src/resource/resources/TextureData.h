#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <string>

#include "Handle.h"
#include "IResource.h"
#include "ResourceMacros.h"
#include "Texture_Asset.h"

struct TextureData : public IResource{
    RESOURCE_CLASS(TextureData)
public:
    TextureData(uint32_t _width, uint32_t _height, uint32_t _channels)
        : width(_width), height(_height), channels(_channels) {}
  uint32_t width;
  uint32_t height;
  uint32_t channels;
  GLuint id = 0;
};
PEARL_DEFINE_RESOURCE(Texture, TextureData, Texture_Asset)

class ResourceSystem;

void BindTexture(ResourceSystem* rs, TextureHandle handle, unsigned int slot);
void UnbindTexture();
void DestroyTexture(ResourceSystem* rs, TextureHandle handle);

