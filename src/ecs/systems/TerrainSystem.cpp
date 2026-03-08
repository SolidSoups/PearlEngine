#include "TerrainSystem.h"

#include <functional>

#include <glm/glm.hpp>

#include "ecs_system_impl.h"
#include "TransformComponent.h"
#include "TerrainComponent.h"
#include "ServiceLocator.h"
#include "ShaderManager.h"
#include "MaterialLoader.h"

#include "Renderer.h"

#define TERRAIN_VERT_PATH "shaders/terrainVert.glsl"
#define TERRAIN_FRAG_PATH "shaders/terrainFrag.glsl"

void TerrainSystem::render(){
  if(!myTerrainShader){
    if(ServiceLocator::IsReady<ShaderManager>()){
      myTerrainShader = ServiceLocator::Get<ShaderManager>().load(
        TERRAIN_VERT_PATH, TERRAIN_FRAG_PATH
      );
      LOG_INFO << "Loaded terrain shader";
    }
    else return;
  }

  for(auto entity : Entities){
    auto& transform = Get<TransformComponent>(entity);
    auto& terrain = Get<TerrainComponent>(entity);
    if(!terrain.mesh or !terrain.heightMap or !terrain.material)
      continue;

    if(!terrain.material->getShader()){
        terrain.material->setShader(myTerrainShader);
    }

    // do we need this?
    Renderer::Submit(transform, terrain.mesh, terrain.material);   
  }
}

void TerrainSystem::generateValidTerrains(){
  for(ecs::Entity e : Entities){
    // get terrain and transform components
    auto& transform = Get<TransformComponent>(e);
    auto& terrain = Get<TerrainComponent>(e);

    // can't generate without heightmap
    if(!terrain.heightMap) continue;
    // skip valid meshes, but regenerate if terrain is dirty
    if(terrain.mesh && !terrain.isDirty) continue;
  
    generateTerrain(transform, terrain);
    configureTerrainMaterial(terrain);  

    terrain.isDirty = false;
  } 
}

void TerrainSystem::configureTerrainMaterial(TerrainComponent& aTerrain){
  // create a material if it doesn't exist
  // otherwise, ensure correct shader
  if(!aTerrain.material)
    aTerrain.material = std::make_shared<Material>(myTerrainShader);
  else
    aTerrain.material->setShader(myTerrainShader);

  // link terrain heightMap texture to material uniform
  aTerrain.material->setTexture("uHeightMap", aTerrain.heightMap);
}

void TerrainSystem::generateTerrain(const TransformComponent& aTransform, 
                     TerrainComponent& aTerrain){
  // no height map, no generation
  if(!aTerrain.heightMap){
    LOG_ERROR << "Cannot generate terrain without a height map";
    return;
  }

  // try load height map data (red channel only)
  auto img = aTerrain.heightMap->getPixelData(1);
  if (!img) {
    LOG_ERROR << "Failed to load pixels for height map";
    return;
  }

  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  generateVertices(*img, vertices, aTerrain.resolution); 
  generateIndices(indices, aTerrain.resolution, aTerrain.resolution);
  // create mesh
  aTerrain.mesh = std::make_shared<Mesh>(vertices, indices);

  LOG_INFO << "Terrain generated";
}

// sample an stbi image at a pixel coordinate
float sampleHeightMap(const StbiImage &heightMap, glm::ivec2 coord){
  coord = glm::clamp(coord, glm::ivec2(0), glm::ivec2(heightMap.width-1, heightMap.height-1));
  return heightMap.data[coord.x + coord.y * heightMap.width] / 255.0f;
}

// get the normal of a coordinate using pixel coordinates
glm::vec3 calculateHeightMapNormal(const StbiImage& heightMap, glm::ivec2 coord, glm::ivec2 cellSize){
  constexpr float normalStrength = 2.f;
  float up = sampleHeightMap(heightMap, coord + glm::ivec2(0, cellSize.y));
  float right = sampleHeightMap(heightMap, coord + glm::ivec2(cellSize.x, 0));
  float down = sampleHeightMap(heightMap, coord + glm::ivec2(0, -cellSize.y));
  float left = sampleHeightMap(heightMap, coord + glm::ivec2(-cellSize.x, 0));
  return glm::normalize(glm::vec3(left - right, normalStrength, up - down));
}

void TerrainSystem::generateVertices(const StbiImage &heightMap, std::vector<float> &outVertices, size_t resolution){
  outVertices.clear();
  outVertices.reserve(resolution * resolution * 8); // 9 floats (pos3, uv2, normal3) per cell

  // create the vertices. We iterate over a grid
  glm::vec3 startPos(-0.5f);
  int widthStep = heightMap.width / resolution;
  int heightStep = heightMap.height / resolution;
  for (int y = 0; y < resolution; y++) {
    for (int x = 0; x < resolution; x++) {
      // sample height map
      int px = widthStep * x;
      int py = heightStep * y;
      float heightSample = sampleHeightMap(heightMap, glm::ivec2(px, py));

      // create vertex attributes
      glm::vec2 vertexUV((float)px / heightMap.width, (float)py / heightMap.height);
      glm::vec3 vertexPos(vertexUV.x + startPos.x,
                          heightSample,
                          vertexUV.y + startPos.z);
      glm::vec3 vertexNormal = calculateHeightMapNormal(heightMap, glm::ivec2(px, py), glm::ivec2(widthStep, heightStep));

      // add vertex to list
      outVertices.push_back(vertexPos.x);
      outVertices.push_back(vertexPos.y);
      outVertices.push_back(vertexPos.z);
      outVertices.push_back(vertexUV.x);
      outVertices.push_back(vertexUV.y);
      outVertices.push_back(vertexNormal.x);
      outVertices.push_back(vertexNormal.y);
      outVertices.push_back(vertexNormal.z);
    }
  }
}

void TerrainSystem::generateIndices( std::vector<unsigned int>& outIndices, size_t columns, size_t rows){
  outIndices.clear();
  outIndices.reserve((columns-1)*(rows-1)*6); // 6 indices (2 triangles) per face
  
  // using coordinate system where (0,0) is top left
  
  for(size_t y = 0; y < rows-1; y++){
    for(size_t x = 1; x < columns; x++){
      // get this index and the index of the cell right below
      size_t cellIndex = x + y * columns;  
      size_t cellBelowIndex = x + (y+1) * columns; 

      // top left triangle
      outIndices.push_back(cellIndex);  
      outIndices.push_back(cellIndex-1);  
      outIndices.push_back(cellBelowIndex-1);  

      // bottom right triangle
      outIndices.push_back(cellIndex);  
      outIndices.push_back(cellBelowIndex-1);  
      outIndices.push_back(cellBelowIndex);  
    }
  }
}
