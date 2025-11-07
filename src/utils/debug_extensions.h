#pragma once

#include <glm/glm.hpp>
#include <iomanip>
#include <iostream>

inline std::ostream &operator<<(std::ostream &os, const glm::vec3 &v) {
  return os << "vec3(" << std::fixed << std::setprecision(1) << v.x << ", "
            << v.y << ", " << v.z << ")";
}
