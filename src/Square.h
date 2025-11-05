#pragma once
#include "Renderable.h"

class Square : public Renderable{
public:
  Square();
  ~Square();
  void Render(Shader& shader);

private:
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
};
