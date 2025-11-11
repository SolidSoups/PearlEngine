#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount)
  : m_VertexCount(vertexCount)
  , m_IndexCount(indexCount)
{
  glGenVertexArrays(1, &VAO); // generate vertex array object
  glBindVertexArray(VAO); // bind the VAO in openGL's state machine

  glGenBuffers(1, &VBO); // generate buffers for the vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO to the target GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, vertexCount * 8 * sizeof(float), vertices, GL_STATIC_DRAW); // feed the target of GL_ARRAY_BUFFER (which is our VBO) the data for the vertices

  EBO = 0; // initialize EBO as 0 incase we don't use it
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
}

void Mesh::Render(){
  BindMaterial(m_MaterialHandle);

  glBindVertexArray(VAO);  

  if(EBO){
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
  }
  else{
    glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
  }

  glBindVertexArray(0);
}

void Mesh::SetMaterial(MaterialHandle handle){
  m_MaterialHandle = handle;
}
