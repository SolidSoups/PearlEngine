#include "Scene.h"

#include <iostream>
#include <algorithm>
#include <string>

#include "PointLightComponent.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "PointLightSystem.h"
#include "ScriptComponent.h"

#include "TransformComponent.h"
#include "NameComponent.h"
#include "Mesh.h"
#include "Cube.h"
#include "json_common.h"
#include "Geometry.h"

#include "FileSystem.h"

Scene::Scene() {
  m_Coordinator.Init();

  // Register all component types
  m_Coordinator.RegisterComponent<TransformComponent>();
  m_Coordinator.RegisterComponent<RenderComponent>();
  m_Coordinator.RegisterComponent<CameraComponent>();
  m_Coordinator.RegisterComponent<PointLightComponent>();
  m_Coordinator.RegisterComponent<NameComponent>();
  m_Coordinator.RegisterComponent<ScriptComponent>();

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

  // ScriptSystem
  mScriptSystem = m_Coordinator.RegisterSystem<ScriptSystem>();
  ecs::Signature ssSignature;
  ssSignature.set(m_Coordinator.GetComponentType<ScriptComponent>());
  m_Coordinator.SetSystemSignature<ScriptSystem>(ssSignature);

  mScriptEngine = std::make_shared<ScriptEngine>();
  mScriptEngine->Init(this);
  mScriptSystem->Init(&m_Coordinator, mScriptEngine.get());
}

void Scene::DestroyEntity(ecs::Entity entity) {
  m_Coordinator.DestroyEntity(entity);
  m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity),
                   m_Entities.end());
}

void Scene::Clear() {
  mScriptSystem->OnDestroy();
  for (auto entity : m_Entities) {
    m_Coordinator.DestroyEntity(entity);
  }
  m_Entities.clear();
  m_ActiveCamera = ecs::NULL_ENTITY;
}

void Scene::Update() {
  mScriptSystem->OnUpdate();
}

void Scene::Render(Camera &camera) {

  // pass 1, upload generic data
  Renderer::BeginScene(camera);
  mRenderSystem->RenderAll();
  Renderer::EndScene();
}

std::string Scene::GetEntityName(ecs::Entity entity) {
  if (m_Coordinator.HasComponent<NameComponent>(entity)) {
    return m_Coordinator.GetComponent<NameComponent>(entity).name;
  }
  return "Entity_" + std::to_string(entity);
}

void Scene::SetEntityName(ecs::Entity entity, const std::string &name) {
  if (m_Coordinator.HasComponent<NameComponent>(entity)) {
    m_Coordinator.GetComponent<NameComponent>(entity).name = name;
  }
}

void Scene::SetActiveCamera(ecs::Entity cameraEntity) {
  // unmark previous camera
  if (m_ActiveCamera != ecs::NULL_ENTITY &&
      m_Coordinator.HasComponent<CameraComponent>(m_ActiveCamera)) {
    m_Coordinator.GetComponent<CameraComponent>(m_ActiveCamera).isMainCamera =
        false;
  }

  // set new camera
  m_ActiveCamera = cameraEntity;
  if (m_ActiveCamera != ecs::NULL_ENTITY &&
      m_Coordinator.HasComponent<CameraComponent>(m_ActiveCamera)) {
    m_Coordinator.GetComponent<CameraComponent>(m_ActiveCamera).isMainCamera =
        true;
  }
}

std::vector<ecs::Entity> Scene::GetPointLightEntities() const {
  std::vector<ecs::Entity> lights;
  for (auto entity : m_Entities) {
    if (const_cast<ecs::Coordinator &>(m_Coordinator)
            .HasComponent<PointLightComponent>(entity)) {
      lights.push_back(entity);
    }
  }
  return lights;
}

ecs::Entity Scene::CreateEntity(const std::string &name) {
  ecs::Entity entity = m_Coordinator.CreateEntity();
  m_Coordinator.AddComponent(entity, NameComponent{name});
  m_Coordinator.AddComponent(entity, TransformComponent{});
  m_Entities.push_back(entity);
  return entity;
}

ecs::Entity Scene::CreatePointLight(const std::string &name) {
  ecs::Entity entity = CreateEntity(name);
  m_Coordinator.AddComponent(entity, PointLightComponent{});
  return entity;
}

ecs::Entity Scene::CreateCube(const std::string &name) {
  ecs::Entity entity = CreateEntity(name);
  RenderComponent renderComp;
  std::vector<float> vertices(Cube::s_Vertices,
                              Cube::s_Vertices + Cube::s_VertexCount * 8);
  std::vector<unsigned int> indices(Cube::s_Indices,
                                    Cube::s_Indices + Cube::s_IndexCount);
  renderComp.mesh = std::make_shared<Mesh>(vertices, indices);
  m_Coordinator.AddComponent(entity, renderComp);
  return entity;
}

ecs::Entity Scene::CreatePlane(const std::string& name){
  // load vertices into vectors
  std::vector<float> verts(
    Plane::vertices, 
    Plane::vertices + PLANE_VERTEX_SIZE);
  std::vector<unsigned int> indcs(
    Plane::indices, 
    Plane::indices + PLANE_INDEX_SIZE);
  
  // create rendercomp, attach mesh
  RenderComponent renderComp;
  renderComp.mesh = std::make_shared<Mesh>(verts, indcs);

  // create entity, attach rendercomp
  ecs::Entity entity = CreateEntity(name);
  m_Coordinator.AddComponent(entity, renderComp);

  return entity;
}

ecs::Entity Scene::CreateSphere(const std::string& name){
  // generate mesh data
  std::vector<float> verts;
  std::vector<unsigned int> indices;
  Sphere::generate_uvsphere(verts, indices, 24, 16);

  // create render comp and make a mesh
  LOG_INFO << "Creating mesh object";
  RenderComponent renderComp;
  renderComp.mesh = std::make_shared<Mesh>(verts, indices);
  LOG_INFO << "Mesh object created!";

  // create entity, attach rendercomp
  ecs::Entity entity = CreateEntity(name);
  m_Coordinator.AddComponent(entity, renderComp);

  return entity;
}





void Scene::SaveScene(const char* filepath) {
  json scene;
  scene["name"] = "Test Scene 1";
  json &entities = scene["entities"];

  size_t counter = 0;
  for (unsigned int entity : m_Entities) {
    std::string name = m_Coordinator.GetComponent<NameComponent>(entity).name;
    json& entity_object = entities[name + std::to_string(counter)];
    entity_object["name"] = name;

    // add components
    if (m_Coordinator.HasComponent<TransformComponent>(entity))
      entity_object["transform_component"] = m_Coordinator.GetComponent<TransformComponent>(entity);
    if (m_Coordinator.HasComponent<RenderComponent>(entity))
      entity_object["render_component"] = m_Coordinator.GetComponent<RenderComponent>(entity);
    if (m_Coordinator.HasComponent<CameraComponent>(entity))
      entity_object["camera_component"] = m_Coordinator.GetComponent<CameraComponent>(entity);
    if (m_Coordinator.HasComponent<PointLightComponent>(entity))
      entity_object["pointLight_component"] = m_Coordinator.GetComponent<PointLightComponent>(entity);

    counter++;
  }

  std::string json_str = scene.dump(2);

  // copy to vector
  std::vector<char> bytes(json_str.begin(), json_str.end());

  FileSystem::writeFile(filepath, bytes);
  LOG_INFO << "Saved scene to: " << filepath;
}

void Scene::LoadScene(const char *filepath) {
  Clear();

  std::vector<char> bytes;
  if (!FileSystem::loadFile(filepath, bytes)) {
    LOG_ERROR << "Could not load scene";
    return;
  }

  json j;
  try {
    j = json::parse(bytes);
  } catch (json::parse_error &e) {
    LOG_ERROR << "nlohmann::json::parse_error: \n"
              << e.byte << ": " << e.what();
    return;
  }

  if(!j.contains("entities")){
    LOG_ERROR << "Could not find entities field";
    return;
  }

  for(const auto& [e, c] : j["entities"].items()){
    ecs::Entity entity = m_Coordinator.CreateEntity();
    m_Coordinator.AddComponent<NameComponent>(entity, NameComponent{c["name"]});
    
    if(c.contains("transform_component"))
      m_Coordinator.AddComponent<TransformComponent>(entity, c["transform_component"]);
    if(c.contains("render_component"))
      m_Coordinator.AddComponent<RenderComponent>(entity, c["render_component"]);
    if(c.contains("camera_component"))
      m_Coordinator.AddComponent<CameraComponent>(entity, c["camera_component"]);
    if(c.contains("pointLight_component"))
      m_Coordinator.AddComponent<PointLightComponent>(entity, c["pointLight_component"]);

    m_Entities.push_back(entity);
  }
  LOG_INFO << "Loaded scene from: " << filepath;
}
