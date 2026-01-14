#include "MaterialLoader.h"

#include "Material.h"
#include "ShaderData.h"


std::shared_ptr<Material> MaterialLoader::create(std::shared_ptr<ShaderData> shader){
  return std::make_shared<Material>(shader);
}
