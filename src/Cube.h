#pragma once
#include "Renderable.h"

class Cube : public Renderable{
public:
  Cube();
  ~Cube();
public:
  void Render(Shader& shader);

private:
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
};
