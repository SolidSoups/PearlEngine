#pragma once

#include "json_common.h"
#include "Material.h"
#include "MeshManager.h"
#include "Material.h"
#include "ServiceLocator.h"
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

inline void to_json(json& j, const RenderComponent& cmp){
  if(cmp.mesh)
    j["mesh_filepath"] = cmp.mesh->getFilePath();
  if(cmp.material)
    j["material_asset"] = cmp.material->createConstruction();
}

inline void from_json(const json& j, RenderComponent& cmp){
  if(j.contains("mesh_filepath")){
    std::string filepath = j["mesh_filepath"];
    cmp.mesh = ServiceLocator::Get<MeshManager>().loadOBJ(filepath.c_str());
  }
  if(j.contains("material_asset")){
    cmp.material = Material::createDefault();
    Material::ConstructData data = j["material_asset"];  
    cmp.material->fromConstruction(data); 
  }
}
