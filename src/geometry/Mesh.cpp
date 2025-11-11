#include "Mesh.h"
#include "MeshData.h"
#include <glad/glad.h>

Mesh::Mesh(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount)
{
  m_MeshHandle = CreateMesh(vertices, vertexCount, indices, indexCount);
}

Mesh::~Mesh(){
  DestroyMesh(m_MeshHandle);
}

void Mesh::Render(){
  RenderMesh(m_MeshHandle);
}
