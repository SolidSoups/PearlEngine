#include "Scene.h"

#include <iostream>
#include <algorithm>

#include "PointLightComponent.h"
#include "RenderComponent.h"
#include "Renderer.h"

#include "GameObject.h"
#include "TransformComponent.h"

GameObject* Scene::CreateGameObject(const std::string& name){
  auto newGO = std::make_unique<GameObject>(m_NextObjectID++, name);
  newGO->AddComponent<TransformComponent>();
  GameObject* ptr = newGO.get();
  m_GameObjects.push_back(std::move(newGO));
  return ptr;
}
GameObject* Scene::CreatePointLight(const std::string& name){
  auto newGO = std::make_unique<GameObject>(m_NextObjectID++, name);
  newGO->AddComponent<TransformComponent>();
  newGO->AddComponent<PointLightComponent>();
  GameObject* ptr = newGO.get();
  m_GameObjects.push_back(std::move(newGO));
  return ptr;
}

void Scene::Update(){
  // in the future we update the objects here
}

void Scene::Render(Camera& camera){

  // pass 1, upload generic data
  Renderer::BeginScene(camera, ambientLight);
  Renderer::SubmitLights(GetPointLights());

  for(auto& object : m_GameObjects){
    auto transform = object->GetComponent<TransformComponent>();
    auto renderComp = object->GetComponent<RenderComponent>();

    if(transform && renderComp)
      Renderer::Submit(*renderComp, *transform);
  }
  Renderer::EndScene();
}

const std::vector<std::unique_ptr<GameObject>>& GetGameObjects();

void Scene::SetActiveCamera(CameraComponent* camera) {
  // unmark previous camera
  if(m_ActiveCamera){
    m_ActiveCamera->isMainCamera = false;
  }

  // set new camera
  m_ActiveCamera = camera;
  if(m_ActiveCamera){
    m_ActiveCamera->isMainCamera = true;
  }
}

std::vector<PointLightComponent*> Scene::GetPointLights() const {
  std::vector<PointLightComponent*> lights;
  for(auto& object : m_GameObjects){
    auto light = object->GetComponent<PointLightComponent>();
    if(light){
      lights.push_back(light);
    }
  }
  return lights;
}
