#include "MaterialLoader.h"

#include "MaterialData.h"
#include "ShaderData.h"


std::shared_ptr<MaterialData> MaterialLoader::create(std::shared_ptr<ShaderData> shader){
  return std::make_shared<MaterialData>(shader);
}
