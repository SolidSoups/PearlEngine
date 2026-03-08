#pragma once

#include "ecs_system.h"

struct StbiImage;
struct TerrainComponent;
struct TransformComponent;
struct ShaderData;
class Material;

class TerrainSystem : public ecs::System {
public:
  void generateTerrain(const TransformComponent& aTransform, 
                       TerrainComponent& aTerrain);
  void render();

  auto getShader() const { return myTerrainShader; }

  // Generates meshes for any terrains that
  // have a valid height map and transform component
  void generateValidTerrains();

private:
  void configureTerrainMaterial(TerrainComponent& aTerrain);
  void generateVertices(const StbiImage &heightMap, std::vector<float> &outVertices, size_t resolution);
  void generateIndices(std::vector<unsigned int>& outIndices, size_t columns, size_t rows);

private:
  std::shared_ptr<ShaderData> myTerrainShader;
};
