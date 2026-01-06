#pragma once

#include "MaterialData.h"
#include "ShaderData.h"

class ResourceSystem;

MaterialHandle CreateMaterial(ResourceSystem* rs, ShaderHandle shaderHandle);

