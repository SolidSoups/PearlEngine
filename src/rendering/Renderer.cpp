#include "Renderer.h"
#include "AmbientLightData.h"
#include "Camera.h"
#include "Logger.h"
#include "Material.h"
#include "ShaderData.h"
#include "Mesh.h"
#include "Scene.h"
#include <vector>

Camera *Renderer::s_ActiveCamera = nullptr;
AmbientLightData Renderer::m_AmbientData;
std::vector<ecs::Entity> Renderer::m_PointLightEntities;
Scene* Renderer::m_Scene = nullptr;
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

void Renderer::SubmitLights(Scene& scene) {
  m_Scene = &scene;
  m_PointLightEntities = scene.GetPointLightEntities();
}

void Renderer::EndScene() {
  s_ActiveCamera = nullptr;
  m_Scene = nullptr;
}

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

  if (!m_Scene) return;

  auto& coordinator = m_Scene->GetCoordinator();

  // upload light
  shader->setInt("numPointLights", m_PointLightEntities.size());
  for (size_t i = 0; i < m_PointLightEntities.size(); i++) {
    ecs::Entity entity = m_PointLightEntities[i];
    auto& light = coordinator.GetComponent<PointLightComponent>(entity);
    auto& transform = coordinator.GetComponent<TransformComponent>(entity);

    std::string base = "pointLights[" + std::to_string(i) + "].";
    shader->setVec3((base + "position").c_str(), transform.position);
    shader->setVec3((base + "color").c_str(), glm::vec3(light.data.color));
    shader->setFloat((base + "intensity").c_str(), light.data.intensity);
    shader->setFloat((base + "range").c_str(), light.data.radius);
    shader->setFloat((base + "constant").c_str(),
                     light.data.constantAttenuation);
    shader->setFloat((base + "linear").c_str(), light.data.linearAttenuation);
    shader->setFloat((base + "quadratic").c_str(),
                     light.data.quadraticAttenuation);
  }
}
