#include "Geometry.h"

// Plane
const float Plane::vertices[] = {
  // position           // uv         // normal
  -0.5f,  0.0f,  0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
  0.5f,  0.0f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
  -0.5f,  0.0f, -0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
  0.5f,  0.0f, -0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f
};
const unsigned int Plane::indices[] = {
  0, 1, 2,
  2, 1, 3
};
