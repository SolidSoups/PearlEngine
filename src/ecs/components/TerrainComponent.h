#pragma once

#include "TextureData.h"
#include "Mesh.h"
#include "json_common.h"

struct TerrainComponent {
  std::shared_ptr<TextureData> heightMap;
  glm::vec3 size{1.0f};
  int resolution = 20;

public: // runtime, not serialized
  std::shared_ptr<Mesh> myMesh;

public: // serialization
  friend void to_json(json& j, const TerrainComponent&t);
  friend void from_json(const json& j, TerrainComponent&t);
};



