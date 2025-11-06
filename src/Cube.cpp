#include "Cube.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static float vertices[] = {
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

static unsigned int indices[] = {
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

Cube::Cube(){
  glGenVertexArrays(1, &VAO); 
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  std::cout << "Cube::Cube() -> Created a cube" << std::endl;
}

Cube::~Cube(){
  if(VBO) glDeleteBuffers(1, &VBO);
  if(EBO) glDeleteBuffers(1, &EBO);
  if(VAO) glDeleteVertexArrays(1, &VAO);

  std::cout << "Cube::~Cube() -> Deleted VBO, VAO and EBO" << std::endl;
}

void Cube::Render(Shader& shader){
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
