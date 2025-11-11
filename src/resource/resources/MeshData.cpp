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

MeshHandle CreateMesh(
  const float* vertices, 
  size_t vertexCount, 
  const unsigned int* indices, 
  size_t indexCount){
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;

  glGenVertexArrays(1, &VAO); // generate vertex array object
  glBindVertexArray(VAO); // bind the VAO in openGL's state machine

  glGenBuffers(1, &VBO); // generate buffers for the vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO to the target GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, vertexCount * 8 * sizeof(float), vertices, GL_STATIC_DRAW); // feed the target of GL_ARRAY_BUFFER (which is our VBO) the data for the vertices

  // only generate Element buffer object if we need it...
  if(indices && indexCount > 0){ 
    glGenBuffers(1, &EBO); // generate a buffer for EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the buffer to target GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW); // feed the buffer some data (mmmmmmmm)
  }

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // uv attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // reset bindings
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // create mesh data and get handle
  MeshHandle meshHandle = ResourceSystem::Get().Meshes().Create({VAO, VBO, EBO, vertexCount, indexCount});

  return meshHandle;
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
