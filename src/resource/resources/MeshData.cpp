#include "MeshData.h"
#include "ResourceSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace {
#define GET_MESH_OR_NULL(handle) GetMeshDataOrNull(handle, __func__)
MeshData* GetMeshDataOrNull(MeshHandle handle, const char* functionName){
  MeshData* meshData = ResourceSystem::Get().Meshes().Get(handle);
  if(!meshData)
    std::cerr << "MeshData.cpp: " << functionName << ": MeshData is null" << std::endl;
  return meshData;
};
}

void DestroyMesh(MeshHandle handle){
  MeshData* meshData = GET_MESH_OR_NULL(handle);
  if(!meshData) return;

  // delete objects and buffers
  if(meshData->VAO) glDeleteVertexArrays(1, &meshData->VAO);
  if(meshData->VBO) glDeleteBuffers(1, &meshData->VBO);
  if(meshData->EBO) glDeleteBuffers(1, &meshData->EBO);

  // delete handle
  ResourceSystem::Get().Meshes().Destroy(handle);
}

void RenderMesh(MeshHandle handle){
  MeshData* meshData = GET_MESH_OR_NULL(handle);
  if(!meshData) return;

  glBindVertexArray(meshData->VAO);
  if(meshData->EBO){
    glDrawElements(GL_TRIANGLES, meshData->indexCount, GL_UNSIGNED_INT, 0);
  }
  else{
    glDrawArrays(GL_TRIANGLES, 0, meshData->vertexCount);
  }

  glBindVertexArray(0);
}
