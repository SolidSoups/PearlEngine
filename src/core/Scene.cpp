#include "Scene.h"

#include <iostream>
#include <algorithm>

#include "RenderComponent.h"
#include "Renderable.h"
#include "Renderer.h"

#include "GameObject.h"
#include "TransformComponent.h"

GameObject* Scene::CreateGameObject(){
  auto newGO = std::make_unique<GameObject>(m_NextObjectID++);
  GameObject* ptr = newGO.get();
  m_GameObjects.push_back(std::move(newGO));
  return ptr;
}

void Scene::Update(){
  // in the future we update the objects here
}

void Scene::Render(Camera& camera){
  Renderer::BeginScene(camera);
  for(auto& object : m_GameObjects){
    auto transform = object->GetComponent<TransformComponent>();
    auto renderComp = object->GetComponent<RenderComponent>();

    if(transform && renderComp)
      Renderer::Submit(*renderComp, *transform);
  }
  Renderer::EndScene();
}
