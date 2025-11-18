#include "meshLoaders.h"

#include "Logger.h"
#include "glad/glad.h"

#include "ResourceSystem.h"

MeshHandle CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices){
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;

  glGenVertexArrays(1, &VAO); // generate vertex array object
  glBindVertexArray(VAO); // bind the VAO in openGL's state machine

  glGenBuffers(1, &VBO); // generate buffers for the vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO to the target GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), vertices.data(), GL_STATIC_DRAW); // feed the target of GL_ARRAY_BUFFER (which is our VBO) the data for the vertices

  // only generate Element buffer object if we need it...
  if(indices.size() > 0){ 
    glGenBuffers(1, &EBO); // generate a buffer for EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the buffer to target GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW); // feed the buffer some data (mmmmmmmm)
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
  MeshHandle meshHandle = ResourceSystem::Get().Meshes().Create(
    { VAO, VBO, EBO, vertices.size(), indices.size() }
  );

  LOG_INFO << "Created mesh (id=" << meshHandle.id << ")";

  return meshHandle;
};
