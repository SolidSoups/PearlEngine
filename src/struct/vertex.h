#pragma once

#include "glm/glm.hpp"
#include <ostream>
#include <sstream>

struct Vertex{
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 uv;
};

struct VertexNew{
  glm::vec3 position;
  glm::vec2 uv;
  glm::vec3 normal;
};

inline std::ostream& operator<<(std::ostream& ss, const VertexNew& vertex){
  ss << "(" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << ", "
    << vertex.uv.x << ", " << vertex.uv.y << ", "
    << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << ")";
  return ss; 
}
