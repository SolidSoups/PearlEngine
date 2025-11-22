#include "materialLoaders.h"

#include "Logger.h"
#include "ResourceSystem.h"

MaterialHandle CreateMaterial(ShaderHandle shaderHandle){
  MaterialHandle matHandle = 
    ResourceSystem::Get().Create(MaterialData{shaderHandle});
  LOG_INFO << "Created material (id=" << matHandle.id << ") with shader handle " << shaderHandle.id;
  return matHandle;
}
