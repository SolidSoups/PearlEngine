#pragma once

#include "ShaderData.h"

class ResourceSystem;

ShaderDataHandle CreateShader(ResourceSystem* rs, const char *vertexPath, const char *fragmentPath);
