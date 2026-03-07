#include "TerrainComponent.h"
#include "ServiceLocator.h"
#include "TextureManager.h"

void to_json(json& j, const TerrainComponent&t){
  j["size"] = t.size;
  j["resolution"] = t.resolution;
  if(t.heightMap){
    j["heightMap"] = *t.heightMap;
  }
}
void from_json(const json& j, TerrainComponent&t){
  t.size = j["size"];
  t.resolution = j["resolution"];
  if(j.contains("heightMap")){
    std::string path = j["heightMap"]["filePath"];
    TextureConfig config = j["heightMap"]["config"];
    auto& texManager = ServiceLocator::Get<TextureManager>(); 
    t.heightMap = texManager.load(path.c_str(), config);
  }
}
