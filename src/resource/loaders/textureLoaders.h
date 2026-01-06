#pragma once

#include "TextureData.h"

class ResourceSystem;

TextureHandle LoadTexture(ResourceSystem* rs, const std::string& filepath, bool generateMipMaps = true);
