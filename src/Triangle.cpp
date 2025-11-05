// src
#include "Triangle.h"

// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

Triangle::Triangle() {
  float vertices[] = {
    // positions          // colors
   -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
    0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // location=0

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3* sizeof(float)));
  glEnableVertexAttribArray(1); // location=1

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  std::cout << "Triangle::Triangle() -> Created a triangle" << std::endl;
}

Triangle::~Triangle(){
  if(VBO) glDeleteBuffers(1, &VBO);
  if(VAO) glDeleteVertexArrays(1, &VAO);

  std::cout << "Triangle::~Triangle() -> Deleted VBO and VAO" << std::endl;
}

void Triangle::Render(Shader& shader){
  shader.Use();
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}
