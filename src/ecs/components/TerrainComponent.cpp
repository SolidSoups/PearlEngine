#include "TerrainComponent.h"
#include "ServiceLocator.h"
#include "TextureManager.h"

#include "Material.h"

void to_json(json& j, const TerrainComponent&t){
  j["resolution"] = t.resolution;
  if(t.heightMap){
    j["heightMap"] = *t.heightMap;
  }
  if(t.material){
    j["material_asset"] = *t.material;
  }
}
void from_json(const json& j, TerrainComponent&t){
  t.resolution = j["resolution"];
  if(j.contains("heightMap")){
    std::string path = j["heightMap"]["filePath"];
    TextureConfig config = j["heightMap"]["config"];
    auto& texManager = ServiceLocator::Get<TextureManager>(); 
    t.heightMap = texManager.load(path.c_str(), config);
  }
  if(j.contains("material_asset")){
    t.material = Material::createDefault();
    from_json(j["material_asset"], *t.material);
  }
}
