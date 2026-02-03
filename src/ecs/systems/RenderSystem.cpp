#include "RenderSystem.h"

#include "TransformComponent.h"
#include "RenderComponent.h"

#include "Renderer.h"

#include "Camera.h"
#include "Scene.h"

#include "AmbientLightData.h"

void RenderSystem::Init(ecs::Coordinator* coordinator){
  mCoordinator = coordinator;
}
void RenderSystem::RenderAll(){
  for(auto entity : mEntities){
    auto& transform = mCoordinator->GetComponent<TransformComponent>(entity);
    auto& render = mCoordinator->GetComponent<RenderComponent>(entity);

    Renderer::Submit(render, transform);
  }
}
