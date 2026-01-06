#pragma once

#include "ShaderData.h"

class ResourceSystem;

ShaderHandle CreateShader(ResourceSystem* rs, const char *vertexPath, const char *fragmentPath);
