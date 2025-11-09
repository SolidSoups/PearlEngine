#include "Renderer.h"

#include "Camera.h"
#include "Renderable.h"
#include "Transform.h"
#include "Material.h"

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
  Shader* shader = material->GetShader();
  shader->SetMatrix4(renderable.transform.GetModelMatrix(), "transform");
  shader->SetMatrix4(s_ActiveCamera->GetViewMatrix(), "view");
  shader->SetMatrix4(s_ActiveCamera->GetProjectionMatrix(), "projection");

  // Render the mesh
  renderable.Render();
}
