#include "materialLoaders.h"

#include "Logger.h"
#include "MaterialData.h"
#include "ResourceSystem.h"

MaterialDataHandle CreateMaterial(ResourceSystem* rs, ShaderDataHandle shaderHandle){
    MaterialData* newMatData = new MaterialData(shaderHandle);
    MaterialDataHandle matHandle =
        rs->Create<MaterialData>(newMatData);
  LOG_INFO << "Created material (id=" << matHandle.id << ") with shader handle " << shaderHandle.id;
  return matHandle;
}
