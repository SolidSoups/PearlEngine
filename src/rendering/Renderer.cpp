#include "Renderer.h"

#include "Camera.h"
#include "MaterialData.h"
#include "ShaderData.h"

#include <iostream>

Camera* Renderer::s_ActiveCamera = nullptr;

void Renderer::BeginScene(Camera &camera){
  s_ActiveCamera = &camera;
}

void Renderer::EndScene(){
  s_ActiveCamera = nullptr;
}

void Renderer::Submit(const RenderComponent& renderComp, const TransformComponent& transformComp){
  BindMaterial(renderComp.materialHandle);

  // Set matrices
  ShaderHandle shaderHandle = MaterialGetShaderHandle(renderComp.materialHandle);
  ShaderSetMatrix4(shaderHandle, "transform", transformComp.GetModelMatrix());
  ShaderSetMatrix4(shaderHandle, "view", s_ActiveCamera->GetViewMatrix());
  ShaderSetMatrix4(shaderHandle, "projection", s_ActiveCamera->GetProjectionMatrix());

  // Render the mesh
  RenderMesh(renderComp.meshHandle);
}
