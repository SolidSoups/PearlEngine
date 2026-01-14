#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "ShaderData.h"
#include "Mesh.h"

Camera *Renderer::s_ActiveCamera = nullptr;

void Renderer::BeginScene(Camera &camera) { s_ActiveCamera = &camera; }

void Renderer::EndScene() { s_ActiveCamera = nullptr; }

void Renderer::Submit(const RenderComponent &renderComp,
                      const TransformComponent &transformComp) {
  auto *cameraTarget = s_ActiveCamera->GetCurrentTarget();
  if (!cameraTarget)
    return;

  // Bind material (uploads shader + uniforms + textures)
  if (!renderComp.material)
    return;
  renderComp.material->bind();

  // Set matrices
  auto shader = renderComp.material->getShader();
  if (!shader)
    return;

  glm::vec4 cameraPos = glm::vec4(cameraTarget->position, 1.0f);

  shader->setMatrix4("transform", transformComp.GetModelMatrix());
  shader->setMatrix4("view", cameraTarget->GetViewMatrix());
  shader->setMatrix4("projection", cameraTarget->GetProjectionMatrix());
  shader->setVec4("camera", cameraPos);

  // Render the mesh
  if (renderComp.mesh) {
    renderComp.mesh->Draw();
  }
}
