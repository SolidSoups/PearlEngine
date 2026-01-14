#pragma once

#include "Component.h"
#include "Material.h"
#include "Mesh.h"

// thats it
struct RenderComponent : Component{
  COMPONENT_CLASS(RenderComponent)

public:
  std::shared_ptr<Mesh> mesh;                    // what to render
  std::shared_ptr<Material> material;        // how to render it

  RenderComponent(
    std::shared_ptr<Mesh> _mesh,
    std::shared_ptr<Material> _material
  ) : mesh(_mesh), material(_material) {}
  RenderComponent() : mesh(nullptr), material(Material::createDefault()){}
};
