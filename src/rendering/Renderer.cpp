#include "Renderer.h"
#include "AmbientLightData.h"
#include "Camera.h"
#include "Logger.h"
#include "Material.h"
#include "ShaderData.h"
#include "Mesh.h"
#include <vector>

Camera *Renderer::s_ActiveCamera = nullptr;
AmbientLightData Renderer::m_AmbientData;
std::vector<PointLightComponent *> Renderer::m_PointLights;
bool Renderer::m_bGeometryPassEnabled = false;
std::shared_ptr<ShaderData> Renderer::m_NextShader = nullptr;

void Renderer::SetGeometryPassEnabled(bool value) {
  m_bGeometryPassEnabled = value;
}

void Renderer::SetNextShader(std::shared_ptr<ShaderData> shader) {
  m_NextShader = shader;
}

void Renderer::BeginScene(Camera &camera, const AmbientLightData &ambientData) {
  s_ActiveCamera = &camera;
  m_AmbientData = ambientData;
}

void Renderer::SubmitLights(const std::vector<PointLightComponent *> lights) {
  m_PointLights = lights;
}

void Renderer::EndScene() { s_ActiveCamera = nullptr; }

void Renderer::Submit(const RenderComponent &renderComp,
                      const TransformComponent &transformComp) {
  auto *cameraTarget = s_ActiveCamera->GetCurrentTarget();
  if (!cameraTarget)
    return;

  // Bind material (uploads shader + uniforms + textures)
  std::shared_ptr<ShaderData> shader;
  if (!m_bGeometryPassEnabled) {
    if (!renderComp.material)
      return;
    renderComp.material->bind();
    shader = renderComp.material->getShader();
  } else {
    if (!renderComp.material)
      return;
    shader = m_NextShader;
    shader->use();
    renderComp.material->bind(shader);
  }

  // Set matrices
  if (!shader)
    return;

  // upload object
  shader->setMatrix4("transform", transformComp.GetModelMatrix());
  shader->setMatrix4("view", cameraTarget->GetViewMatrix());
  shader->setMatrix4("projection", cameraTarget->GetProjectionMatrix());

  // Render the mesh
  if (renderComp.mesh) {
    renderComp.mesh->Draw();
  }
}

void Renderer::SendLightUniforms(std::shared_ptr<ShaderData> shader) {
  // set ambient light
  shader->setVec4("ambientColor", m_AmbientData.color);
  shader->setFloat("ambientIntensity", m_AmbientData.intensity);

  // upload light
  shader->setInt("numPointLights", m_PointLights.size());
  for (int i = 0; i < m_PointLights.size(); i++) {
    auto &light = m_PointLights[i];
    // TODO: this should be BUILT into the component parent
    auto transform = light->GetOwner()->GetComponent<TransformComponent>();

    std::string base = "pointLights[" + std::to_string(i) + "].";
    shader->setVec3((base + "position").c_str(), transform->position);
    shader->setVec3((base + "color").c_str(), glm::vec3(light->data.color));
    shader->setFloat((base + "intensity").c_str(), light->data.intensity);
    shader->setFloat((base + "range").c_str(), light->data.radius);
    shader->setFloat((base + "constant").c_str(),
                     light->data.constantAttenuation);
    shader->setFloat((base + "linear").c_str(), light->data.linearAttenuation);
    shader->setFloat((base + "quadratic").c_str(),
                     light->data.quadraticAttenuation);
  }
}
