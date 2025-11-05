// src
#include "Square.h"

// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

Square::Square() {
  float vertices[] = {
    0.7f,  0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,  // bottom right
   -0.7f, -0.5f, 0.0f,  // bottom left
   -0.5f,  0.5f, 0.0f   // top left
  };
  unsigned int indices[] = {
    0, 1, 3,  // first triangle
    1, 2, 3   // second triangle
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  std::cout << "Square::Square() -> Created a triangle" << std::endl;
}

Square::~Square(){
  if(VBO) glDeleteBuffers(1, &VBO);
  if(EBO) glDeleteBuffers(1, &EBO);
  if(VAO) glDeleteVertexArrays(1, &VAO);

  std::cout << "Square::~Square() -> Deleted VBO, VAO and EBO" << std::endl;
}

void Square::Render(Shader& shader){
  // use wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  shader.Use();
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // reset back to default mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
