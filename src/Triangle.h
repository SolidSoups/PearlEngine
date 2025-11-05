#pragma once
#include "Renderable.h"

class Triangle : public Renderable{
public:
  Triangle();
  ~Triangle();
  void Render(Shader& shader);

private:
  unsigned int VBO;
  unsigned int VAO;
};
