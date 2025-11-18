#pragma once

#include "glm/glm.hpp"

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
