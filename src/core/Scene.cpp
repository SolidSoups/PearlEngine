#include "Scene.h"
#include "Renderable.h"
#include "Renderer.h"
#include <algorithm>

void Scene::AddObject(std::unique_ptr<Renderable> object) {
  m_Objects.push_back(std::move(object));
}

void Scene::RemoveObject(Renderable *object) {
  auto it = std::find_if(m_Objects.begin(), m_Objects.end(),
                         [object](const std::unique_ptr<Renderable> &ptr) {
                           return ptr.get() == object;
                         });
  if(it != m_Objects.end()){
    std::swap(*it, m_Objects.back());
    m_Objects.pop_back();
  }
}

void Scene::Clear(){
  m_Objects.clear();
}

void Scene::Update(){
  // in the future we update the objects here
}

void Scene::Render(Camera& camera){
  Renderer::BeginScene(camera);
  for(auto& object : m_Objects){
    Renderer::Submit(*object.get());
  }
  Renderer::EndScene();
}
