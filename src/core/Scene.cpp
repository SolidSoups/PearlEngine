#include "Scene.h"

#include <iostream>
#include <algorithm>

#include "PointLightComponent.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "PointLightSystem.h"

#include "TransformComponent.h"
#include "NameComponent.h"
#include "Mesh.h"
#include "Cube.h"

Scene::Scene() {
  m_Coordinator.Init();

  // Register all component types
  m_Coordinator.RegisterComponent<TransformComponent>();
  m_Coordinator.RegisterComponent<RenderComponent>();
  m_Coordinator.RegisterComponent<CameraComponent>();
  m_Coordinator.RegisterComponent<PointLightComponent>();
  m_Coordinator.RegisterComponent<NameComponent>();

  // Register render system
  mRenderSystem = m_Coordinator.RegisterSystem<RenderSystem>();
  ecs::Signature rsSignature;
  rsSignature.set(m_Coordinator.GetComponentType<RenderComponent>());
  rsSignature.set(m_Coordinator.GetComponentType<TransformComponent>());
  m_Coordinator.SetSystemSignature<RenderSystem>(rsSignature);
  mRenderSystem->Init(&m_Coordinator);

  // Register light system
  mPointLightSystem = m_Coordinator.RegisterSystem<PointLightSystem>();
  ecs::Signature lsSignature;
  lsSignature.set(m_Coordinator.GetComponentType<TransformComponent>());
  lsSignature.set(m_Coordinator.GetComponentType<PointLightComponent>());
  m_Coordinator.SetSystemSignature<PointLightSystem>(lsSignature);
  mPointLightSystem->Init(&m_Coordinator);
}


void Scene::DestroyEntity(ecs::Entity entity) {
  m_Coordinator.DestroyEntity(entity);
  m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity), m_Entities.end());
}

void Scene::Clear() {
  for (auto entity : m_Entities) {
    m_Coordinator.DestroyEntity(entity);
  }
  m_Entities.clear();
  m_ActiveCamera = ecs::NULL_ENTITY;
}

void Scene::Update(){
  // in the future we update the objects here
}

void Scene::Render(Camera& camera){

  // pass 1, upload generic data
  Renderer::BeginScene(camera, ambientLight);
  Renderer::SubmitLights(*this);

  mRenderSystem->RenderAll();
  Renderer::EndScene();
}

std::string Scene::GetEntityName(ecs::Entity entity) {
  if (m_Coordinator.HasComponent<NameComponent>(entity)) {
    return m_Coordinator.GetComponent<NameComponent>(entity).name;
  }
  return "Entity_" + std::to_string(entity);
}

void Scene::SetEntityName(ecs::Entity entity, const std::string& name) {
  if (m_Coordinator.HasComponent<NameComponent>(entity)) {
    m_Coordinator.GetComponent<NameComponent>(entity).name = name;
  }
}

void Scene::SetActiveCamera(ecs::Entity cameraEntity) {
  // unmark previous camera
  if(m_ActiveCamera != ecs::NULL_ENTITY && m_Coordinator.HasComponent<CameraComponent>(m_ActiveCamera)){
    m_Coordinator.GetComponent<CameraComponent>(m_ActiveCamera).isMainCamera = false;
  }

  // set new camera
  m_ActiveCamera = cameraEntity;
  if(m_ActiveCamera != ecs::NULL_ENTITY && m_Coordinator.HasComponent<CameraComponent>(m_ActiveCamera)){
    m_Coordinator.GetComponent<CameraComponent>(m_ActiveCamera).isMainCamera = true;
  }
}

std::vector<ecs::Entity> Scene::GetPointLightEntities() const {
  std::vector<ecs::Entity> lights;
  for(auto entity : m_Entities){
    if(const_cast<ecs::Coordinator&>(m_Coordinator).HasComponent<PointLightComponent>(entity)){
      lights.push_back(entity);
    }
  }
  return lights;
}







ecs::Entity Scene::CreateEntity(const std::string& name){
  ecs::Entity entity = m_Coordinator.CreateEntity();
  m_Coordinator.AddComponent(entity, NameComponent{name});
  m_Coordinator.AddComponent(entity, TransformComponent{});
  m_Entities.push_back(entity);
  return entity;
}

ecs::Entity Scene::CreatePointLight(const std::string& name){
  ecs::Entity entity = CreateEntity(name);
  m_Coordinator.AddComponent(entity, PointLightComponent{});
  return entity;
}

ecs::Entity Scene::CreateCube(const std::string& name){
  ecs::Entity entity = CreateEntity(name);
  RenderComponent renderComp;
  std::vector<float> vertices(Cube::s_Vertices, Cube::s_Vertices + Cube::s_VertexCount);
  std::vector<unsigned int> indices(Cube::s_Indices, Cube::s_Indices + Cube::s_IndexCount);
  renderComp.mesh = std::make_shared<Mesh>(vertices, indices);
  m_Coordinator.AddComponent(entity, renderComp);
  return entity;
}
