#pragma once

#include "TextureData.h"
#include "json_common.h"

#include "Mesh.h"
#include "Material.h"
#include "StbiImage.h"

struct TerrainComponent {
  TerrainComponent() = default;
  std::shared_ptr<Material> material;
  std::shared_ptr<TextureData> heightMap;
  int resolution = 20;
  std::shared_ptr<StbiImage> pixelCache;

public: 
  // runtime, not serialized
  std::shared_ptr<Mesh> mesh;
  bool isDirty = true;

public: // serialization
  friend void to_json(json& j, const TerrainComponent&t);
  friend void from_json(const json& j, TerrainComponent&t);
};



