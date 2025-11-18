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

void DestroyMesh(MeshHandle handle);
void RenderMesh(MeshHandle handle);
