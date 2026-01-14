#pragma once

#include "glm/ext/vector_float4.hpp"

struct AmbientLightData{
  glm::vec4 color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  float intensity = 1.0f;

  AmbientLightData() = default;
};
