#pragma once

#include "TextureData.h"
#include "json_common.h"

#include "Mesh.h"
#include "Material.h"

struct TerrainComponent {
  std::shared_ptr<Material> material;
  std::shared_ptr<TextureData> heightMap;
  int resolution = 20;

public: 
  // runtime, not serialized
  std::shared_ptr<Mesh> mesh;
  bool isDirty = true;

public: // serialization
  friend void to_json(json& j, const TerrainComponent&t);
  friend void from_json(const json& j, TerrainComponent&t);
};



