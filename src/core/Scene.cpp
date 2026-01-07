#include "Scene.h"

#include <iostream>
#include <algorithm>

#include "RenderComponent.h"
#include "Renderer.h"

#include "GameObject.h"
#include "TransformComponent.h"

GameObject* Scene::CreateGameObject(std::string name){
  auto newGO = std::make_unique<GameObject>(m_NextObjectID++, name);
  GameObject* ptr = newGO.get();
  m_GameObjects.push_back(std::move(newGO));
  return ptr;
}

void Scene::Update(){
  // in the future we update the objects here
}

void Scene::Render(ResourceSystem* rs, Camera& camera){
  Renderer::BeginScene(camera);
  for(auto& object : m_GameObjects){
    auto transform = object->GetComponent<TransformComponent>();
    auto renderComp = object->GetComponent<RenderComponent>();

    if(transform && renderComp)
      Renderer::Submit(rs, *renderComp, *transform);
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
