#pragma once

#include "MaterialData.h"
#include "ShaderData.h"

class ResourceSystem;

MaterialDataHandle CreateMaterial(ResourceSystem* rs, ShaderDataHandle shaderHandle);

