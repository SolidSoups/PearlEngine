#pragma once
#include <glm/glm.hpp>

struct ContactPoint {
  glm::vec3 normal;
  float penetration;
  bool hit;
};
