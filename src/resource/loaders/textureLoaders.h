#pragma once

#include "TextureData.h"

class ResourceSystem;

TextureDataHandle LoadTexture(ResourceSystem* rs, const std::string& filepath, bool generateMipMaps = true);
