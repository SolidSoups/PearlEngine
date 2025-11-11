#pragma once

#include <cstddef>
#include <vector>

#include "Handle.h"
#include "MaterialData.h"
#include <glad/glad.h>

struct MeshData{
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;
  size_t vertexCount;
  size_t indexCount;
};
struct MeshTag{};
using MeshHandle = Handle<MeshTag>;

MeshHandle CreateMesh(
  const float* vertices, 
  size_t vertexCount, 
  const unsigned int* indices, 
  size_t indexCount);
void DestroyMesh(MeshHandle handle);
void RenderMesh(MeshHandle handle);
