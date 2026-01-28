#include "Cube.h"
const float Cube::s_Vertices[] = {
  // Positions          // uv         // normal
  //
  // -front face
 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
 -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
  // -back face
  0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
 -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
  // -bottom face
 -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
  0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
  // -top face
 -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
  // -left face
 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
  // -right face
  0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f
};
const size_t Cube::s_VertexCount = 24;
const unsigned int Cube::s_Indices[] =  {
  // Front face
  0, 1, 2, 2, 3, 0,
  // Back face
  4, 5, 6, 6, 7, 4,
  // Bottom face
  8, 9, 10, 10, 11, 8,
  // Top Face
  12, 13, 14, 14, 15, 12,
  // Left face
  16, 17, 18, 18, 19, 16,
  // Right face
  20, 21, 22, 22, 23, 20
};
const size_t Cube::s_IndexCount = 36;
