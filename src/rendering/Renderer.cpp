#include "Renderer.h"

#include "Camera.h"
#include "MaterialData.h"
#include "ResourceSystem.h"
#include "ShaderData.h"
#include "Mesh.h"

#include <iostream>

Camera *Renderer::s_ActiveCamera = nullptr;

void Renderer::BeginScene(Camera &camera) { s_ActiveCamera = &camera; }

void Renderer::EndScene() { s_ActiveCamera = nullptr; }

void Renderer::Submit(ResourceSystem *rs, const RenderComponent &renderComp,
                      const TransformComponent &transformComp) {
  auto *cameraTarget = s_ActiveCamera->GetCurrentTarget();
  if (!cameraTarget)
    return;
  BindMaterial(rs, renderComp.materialHandle);

  // Set matrices
  ShaderDataHandle shaderHandle =
      MaterialGetShaderHandle(rs, renderComp.materialHandle);
  ShaderSetMatrix4(rs, shaderHandle, "transform",
                   transformComp.GetModelMatrix());
  ShaderSetMatrix4(rs, shaderHandle, "view", cameraTarget->GetViewMatrix());
  ShaderSetMatrix4(rs, shaderHandle, "projection", cameraTarget->GetProjectionMatrix());

  // Render the mesh
  Mesh *mesh = rs->Get(renderComp.meshHandle);
  mesh->Draw();
}
