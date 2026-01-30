#pragma once

#include "Material.h"
#include "Mesh.h"

struct RenderComponent {
  std::shared_ptr<Mesh> mesh;                    // what to render
  std::shared_ptr<Material> material;        // how to render it

  RenderComponent(
    std::shared_ptr<Mesh> _mesh,
    std::shared_ptr<Material> _material
  ) : mesh(_mesh), material(_material) {}
  RenderComponent() : mesh(nullptr), material(Material::createDefault()){}
};
