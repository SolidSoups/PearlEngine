#pragma once

#include "Component.h"
#include "MaterialData.h"
#include "Mesh.h"

// thats it
struct RenderComponent : Component{
  COMPONENT_CLASS(RenderComponent)

public:
  std::shared_ptr<Mesh> mesh;                    // what to render
  std::shared_ptr<MaterialData> material;        // how to render it

  RenderComponent(
    std::shared_ptr<Mesh> _mesh,
    std::shared_ptr<MaterialData> _material
  ) : mesh(_mesh), material(_material) {}
};
