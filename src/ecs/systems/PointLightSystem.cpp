#include "PointLightSystem.h"

#include "ecs_common.h"
#include "ecs_coordinator.h"
#include "PointLightComponent.h"
#include "TransformComponent.h"
#include "AmbientLightData.h"
#include "ShaderData.h"

void PointLightSystem::Init(ecs::Coordinator* coordinator){
  mCoordinator = coordinator;
}

void PointLightSystem::SendUniforms(std::shared_ptr<ShaderData> shader){
  shader->setInt("numPointLights", mEntities.size());
  size_t point_light_id = 0;
  for(auto entity : mEntities){
    auto& pointLight = mCoordinator->GetComponent<PointLightComponent>(entity);
    auto& transform = mCoordinator->GetComponent<TransformComponent>(entity);

    std::string base = "pointLights[" + std::to_string(point_light_id++) + "].";
    shader->setVec3((base + "position").c_str(), transform.position);
    shader->setVec3((base + "color").c_str(), glm::vec3(pointLight.data.color));
    shader->setFloat((base + "intensity").c_str(), pointLight.data.intensity);
    shader->setFloat((base + "range").c_str(), pointLight.data.radius);
    shader->setFloat((base + "constant").c_str(), pointLight.data.constantAttenuation);
    shader->setFloat((base + "linear").c_str(), pointLight.data.linearAttenuation);
    shader->setFloat((base + "quadratic").c_str(), pointLight.data.quadraticAttenuation);
  }
}
