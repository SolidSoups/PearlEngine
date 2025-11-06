#pragma once
#include "Renderable.h"
#include "Transform.h"

class Cube : public Renderable{
public:
  Cube();
  ~Cube();
public:
  void Render(Shader& shader);

public:
  Transform transform {};
private:
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
};
