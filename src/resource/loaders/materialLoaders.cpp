#include "materialLoaders.h"

#include "Logger.h"
#include "MaterialData.h"
#include "ResourceSystem.h"

MaterialHandle CreateMaterial(ResourceSystem* rs, ShaderHandle shaderHandle){
    MaterialData* newMatData = new MaterialData(shaderHandle);
    MaterialHandle matHandle =
        rs->Create<MaterialData>(newMatData);
  LOG_INFO << "Created material (id=" << matHandle.id << ") with shader handle " << shaderHandle.id;
  return matHandle;
}
