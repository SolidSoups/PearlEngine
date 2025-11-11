#include "Renderer.h"

#include "Camera.h"
#include "Renderable.h"
#include "Transform.h"
#include "Material.h"
#include "ShaderData.h"
#include "ResourceSystem.h"

#include <iostream>

Camera* Renderer::s_ActiveCamera = nullptr;

void Renderer::BeginScene(Camera &camera){
  s_ActiveCamera = &camera;
}

void Renderer::EndScene(){
  s_ActiveCamera = nullptr;
}

void Renderer::Submit(Renderable &renderable){
  // Get the mesh's material
  Material* material = renderable.GetMaterial();
  if(!material){
    // No material, cannot render
    return;
  }

  material->Bind();

  // Set matrices
  ShaderSetMatrix4(material->GetShaderHandle(), "transform", renderable.transform.GetModelMatrix());
  ShaderSetMatrix4(material->GetShaderHandle(), "view", s_ActiveCamera->GetViewMatrix());
  ShaderSetMatrix4(material->GetShaderHandle(), "projection", s_ActiveCamera->GetProjectionMatrix());

  // Render the mesh
  renderable.Render();
}
