#pragma once

#include <glm/vec4.hpp>

struct PointLightData{
  glm::vec4 color = glm::vec4(1.0, 0.95, 0.9, 1.0f);  
  float intensity = 0.6f;  
  float radius = 5.f;
  float constantAttenuation = 1.0f;
  float linearAttenuation = 0.1f;
  float quadraticAttenuation = 0.001f;
};
