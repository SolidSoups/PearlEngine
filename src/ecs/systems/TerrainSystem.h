#pragma once

#include "ecs_system.h"

struct StbiImage;
struct TerrainComponent;
struct ShaderData;
class Material;

class TerrainSystem : public ecs::System {
public:
  void generateTerrain(ecs::Entity entity);
  void render();

private:
  void generateVertices(const StbiImage &heightMap, std::vector<float> &outVertices, const glm::vec3& size, size_t resolution);
  void generateIndices(std::vector<unsigned int>& outIndices, size_t columns, size_t rows);

private:
  std::shared_ptr<ShaderData> myTerrainShader;
};
