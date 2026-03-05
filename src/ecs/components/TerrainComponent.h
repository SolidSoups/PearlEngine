#pragma once

#include "TextureData.h"

struct TerrainComponent {
  std::shared_ptr<TextureData> heightMap{nullptr};
  glm::vec3 size{0.0f};
};


