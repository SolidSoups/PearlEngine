#include "Renderer.h"
#include "AmbientLightData.h"
#include "Camera.h"
#include "Logger.h"
#include "Material.h"
#include "ShaderData.h"
#include "Mesh.h"
#include "TextMesh.h"
#include "Scene.h"

#include <glm/ext/matrix_clip_space.hpp>

#include <vector>

Camera *Renderer::s_ActiveCamera = nullptr;
glm::mat4 Renderer::s_View = glm::mat4(1.0f);
glm::mat4 Renderer::s_Proj = glm::mat4(1.0f);
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

void Renderer::BeginScene(Camera &camera){
  s_ActiveCamera = &camera;
}

void Renderer::BeginScene(glm::mat4 view, glm::mat4 proj) {
  s_ActiveCamera = nullptr;
  s_View = view;
  s_Proj = proj;
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
  // Resolve view/projection matrices from whichever BeginScene path was used
  glm::mat4 view, proj;
  if (s_ActiveCamera) {
    auto *cameraTarget = s_ActiveCamera->GetCurrentTarget();
    if (!cameraTarget)
      return;
    view = cameraTarget->GetViewMatrix();
    proj = cameraTarget->GetProjectionMatrix();
  } else {
    view = s_View;
    proj = s_Proj;
  }

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
  shader->setMatrix4("view", view);
  shader->setMatrix4("projection", proj);

  // draw the mesh to the screen
  if(renderComp.mesh) renderComp.mesh->Draw();
}


bool Renderer::ResolveCamera(glm::mat4& view, glm::mat4& proj){
  // Resolve view/projection matrices from whichever BeginScene path was used
  if (s_ActiveCamera) {
    auto *cameraTarget = s_ActiveCamera->GetCurrentTarget();
    if (!cameraTarget){
      LOG_ERROR << "No camera target available";
      return false; }
    view = cameraTarget->GetViewMatrix();
    proj = cameraTarget->GetProjectionMatrix();
  } else {
    view = s_View;
    proj = s_Proj;
  }
  return true;
}
void Renderer::Submit(const TransformComponent& aTransform, const std::shared_ptr<Mesh>& aMesh, const std::shared_ptr<Material>& aMaterial){
  glm::mat4 view, proj;
  if(!ResolveCamera(view, proj))
    return;
  
  // Set matrices
  std::shared_ptr<ShaderData> matShader = aMaterial->getShader();
  if (!matShader){
    LOG_ERROR << "NO MAT SHADER";
    return;
  }

  // bind material and shader
  matShader->use();
  aMaterial->bind();

  // upload object, and camera props
  matShader->setMatrix4("transform", aTransform.GetModelMatrix());
  matShader->setMatrix4("view", view);
  matShader->setMatrix4("projection", proj);

  // draw the mesh to the screen
  if(aMesh) aMesh->Draw();
}

void Renderer::Submit(const TransformComponent &aTransform,
                     const std::shared_ptr<TextMesh> &aTextMesh,
                     const std::shared_ptr<Material> &aMaterial,
                     glm::vec2* aViewportSize){
  // Set matrices
  std::shared_ptr<ShaderData> matShader = aMaterial->getShader();
  if (!matShader){
    LOG_ERROR << "NO MAT SHADER";
    return;
  }

  // bind material and shader
  matShader->use();
  aMaterial->bind();

  glm::mat4 ortho = glm::ortho(0.f, aViewportSize->x, aViewportSize->y, 0.f);
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(aTransform.position.x, aTransform.position.y, 0.0));
  model = glm::rotate(model, glm::radians(aTransform.rotation.z), glm::vec3(0, 0, 1));
  model = glm::scale(model, glm::vec3(aTransform.scale.x, aTransform.scale.y, 1.0f));

  // upload object, and camera props
  matShader->setMatrix4("ortho", ortho);
  matShader->setMatrix4("model", model);

  // draw the mesh to the screen
  if(aTextMesh) aTextMesh->Draw();
}


void Renderer::SendAmbientLightUniforms(std::shared_ptr<ShaderData> shader){
  shader->setVec4("ambientColor", m_AmbientData.color);
  shader->setFloat("ambientIntensity", m_AmbientData.intensity);
}

void Renderer::SendLightUniforms(std::shared_ptr<ShaderData> shader) {
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
