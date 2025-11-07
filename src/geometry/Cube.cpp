#include "Cube.h"

const float Cube::s_Vertices[] = {
  // Positions          // Color
  //
  // -front face
 -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

  // -back face
 -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
  0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,

  // -bottom face
 -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

  // -top face
 -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,

  // -left face
 -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,

  // -right face
  0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f
};

const size_t Cube::s_VertexCount = 24;

const unsigned int Cube::s_Indices[] =  {
  // Front face
  0, 2, 1, 2, 0, 3,
  // Back face
  4, 6, 5, 6, 4, 7,
  // Bottom face
  8, 10, 9, 10, 8, 11,
  // Top Face
  12, 14, 13, 14, 12, 15,
  // Left face
  16, 18, 17, 18, 16, 19,
  // Right face
  20, 22, 21, 22, 20, 23
};


const size_t Cube::s_IndexCount = 36;

Cube::Cube()
  : Mesh(s_Vertices, s_VertexCount, s_Indices, s_IndexCount){}
