#include "Scene.h"

#include <iostream>
#include <algorithm>
#include <string>

#include "PointLightComponent.h"
#include "RigidBodyComponent.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "TerrainComponent.h"
#include "ScriptComponent.h"
#include "CameraComponent.h"
#include "CapsuleColliderComponent.h"
#include "Time.h"
#include "Camera.h"
#include "Renderer.h"

#include "RenderSystem.h"
#include "PointLightSystem.h"
#include "SphereColliderComponent.h"
#include "PhysicsSystem.h"
#include "BoxColliderComponent.h"

#include "TransformComponent.h"
#include "NameComponent.h"
#include "Mesh.h"
#include "Cube.h"
#include "json_common.h"
#include "Geometry.h"

#include "FileSystem.h"
#include "SelectionWizard.h"

Scene::~Scene() {
  for (auto entity : m_Entities) {
    if (m_Coordinator.HasComponent<ScriptComponent>(entity)) {
      m_Coordinator.GetComponent<ScriptComponent>(entity).scriptEnv =
          sol::table{};
    }
  }
}

Scene::Scene(const std::shared_ptr<IEngineCamera> &engineCam,
             const std::shared_ptr<InputManager> &inputMan) {
  m_Coordinator.Init();

  // Register all component types
  m_Coordinator.RegisterComponent<TransformComponent>();
  m_Coordinator.RegisterComponent<RenderComponent>();
  m_Coordinator.RegisterComponent<CameraComponent>();
  m_Coordinator.RegisterComponent<PointLightComponent>();
  m_Coordinator.RegisterComponent<NameComponent>();
  m_Coordinator.RegisterComponent<ScriptComponent>();
  m_Coordinator.RegisterComponent<SphereColliderComponent>();
  m_Coordinator.RegisterComponent<BoxColliderComponent>();
  m_Coordinator.RegisterComponent<CapsuleColliderComponent>();
  m_Coordinator.RegisterComponent<RigidBodyComponent>();
  m_Coordinator.RegisterComponent<TerrainComponent>();

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
  mScriptEngine->Init(this, inputMan);
  mScriptSystem->Init(&m_Coordinator, mScriptEngine.get());

  // Camera System
  mCameraSystem = m_Coordinator.RegisterSystem<CameraSystem>();
  ecs::Signature csSignature;
  csSignature.set(m_Coordinator.GetComponentType<TransformComponent>());
  csSignature.set(m_Coordinator.GetComponentType<CameraComponent>());
  m_Coordinator.SetSystemSignature<CameraSystem>(csSignature);
  mCameraSystem->Init(&m_Coordinator, engineCam);


  // Terrain system
  mTerrainSystem = m_Coordinator.RegisterSystem<TerrainSystem>();
  ecs::Signature requiredComps;
  requiredComps.set(m_Coordinator.GetComponentType<TransformComponent>());
  requiredComps.set(m_Coordinator.GetComponentType<TerrainComponent>());
  m_Coordinator.SetSystemSignature<TerrainSystem>(requiredComps);

  // PhysicsSystem
  mPhysicsSystem = m_Coordinator.RegisterSystem<PhysicsSystem>();
  ecs::Signature physicsRequirements;
  physicsRequirements.set(m_Coordinator.GetComponentType<TransformComponent>());
  ecs::Signature physicsInterest;
  physicsInterest.set(
      m_Coordinator.GetComponentType<SphereColliderComponent>());
  physicsInterest.set(m_Coordinator.GetComponentType<BoxColliderComponent>());
  physicsInterest.set(
      m_Coordinator.GetComponentType<CapsuleColliderComponent>());
  physicsInterest.set(m_Coordinator.GetComponentType<RigidBodyComponent>());
  m_Coordinator.SetSystemInterestSignature<PhysicsSystem>(physicsInterest);
  m_Coordinator.SetSystemSignature<PhysicsSystem>(physicsRequirements);
  mPhysicsSystem->Init(mScriptSystem.get(), mTerrainSystem.get());
}

void Scene::DestroyEntity(ecs::Entity entity) {
  m_Coordinator.DestroyEntity(entity);
  m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity),
                   m_Entities.end());
}

void Scene::DestroyEntityDelayed(ecs::Entity entity) {
  mDestroyQueue.push_back(entity);
}

void Scene::ClearAllEntities() {
  SelectionWizard::Clear();
  mScriptSystem->OnDestroy();
  for (auto entity : m_Entities) {
    m_Coordinator.DestroyEntity(entity);
  }
  m_Entities.clear();
  m_ActiveCamera = ecs::NULL_ENTITY;
  mCameraSystem->Reset();
}

void Scene::OnRuntimeStart(){
  mSceneSnapshot = CreateSceneJSON();
  mScriptSystem->OnCreateScene();
}
void Scene::OnRuntimeStop(){
  // mScriptSystem->OnDestroyScene();
  LoadSceneJSON(mSceneSnapshot);
  mSceneSnapshot = {};
}
void Scene::OnSceneReload(){
  LoadSceneJSON(mSceneSnapshot);
}
void Scene::ReloadCurrentScene(){
  LoadSceneJSON(mCurrentSceneSnapshot);
}


void Scene::Update() {
  mScriptSystem->OnUpdate();

  // physics steps
  constexpr float TIME_STEP = 1 / 60.f;
  static float accumulator = 0;
  accumulator += Time::deltaTime;
  while (accumulator >= TIME_STEP) {
    mPhysicsSystem->UpdatePhysics(TIME_STEP);
    accumulator -= TIME_STEP;
  }

  mScriptSystem->OnLateUpdate();

  // destroy entities at end of frame
  // DOESNT WORK!
  // for(auto& entity : mDestroyQueue){
  //   DestroyEntity(entity);
  // }
}

void Scene::Render(CameraSystem::CameraMode mode) {
  // This should really be somewhere else, 
  // but its fine for a project like this
  mTerrainSystem->generateValidTerrains();

  // get matrices from camera system
  glm::mat4 view, proj;
  mCameraSystem->GetMatrices(view, proj);

  // render everything
  Renderer::BeginScene(view, proj);
  mRenderSystem->RenderAll();
  mTerrainSystem->render();
  Renderer::EndScene();


  // render gizmos
  mPhysicsSystem->DrawGizmos();
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

  mCameraSystem->SetActiveCamera(cameraEntity);
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
  renderComp.meshType = "cube";
  m_Coordinator.AddComponent(entity, renderComp);
  return entity;
}

ecs::Entity Scene::CreatePlane(const std::string &name) {
  // load vertices into vectors
  std::vector<float> verts(Plane::vertices,
                           Plane::vertices + PLANE_VERTEX_SIZE);
  std::vector<unsigned int> indcs(Plane::indices,
                                  Plane::indices + PLANE_INDEX_SIZE);

  // create rendercomp, attach mesh
  RenderComponent renderComp;
  renderComp.mesh = std::make_shared<Mesh>(verts, indcs);
  renderComp.meshType = "plane";

  // create entity, attach rendercomp
  ecs::Entity entity = CreateEntity(name);
  m_Coordinator.AddComponent(entity, renderComp);

  return entity;
}


ecs::Entity Scene::CreateSphere(const std::string &name) {
  // generate mesh data
  std::vector<float> verts;
  std::vector<unsigned int> indices;
  Sphere::generate_uvsphere(verts, indices, 24, 16);

  // create render comp and make a mesh
  LOG_INFO << "Creating mesh object";
  RenderComponent renderComp;
  renderComp.mesh = std::make_shared<Mesh>(verts, indices);
  renderComp.meshType = "sphere";
  LOG_INFO << "Mesh object created!";

  // create entity, attach rendercomp
  ecs::Entity entity = CreateEntity(name);
  m_Coordinator.AddComponent(entity, renderComp);

  return entity;
}

ecs::Entity Scene::DuplicateEntity(ecs::Entity entity) {
  const json j = CreateJSONFromEntity(entity);
  ecs::Entity newEntity = CreateEntityFromJSON(j);
  return newEntity;
}

void Scene::SaveScene(const char *filepath) {
  json scene = CreateSceneJSON();
  std::string json_str = scene.dump(2);
  mCurrentSceneSnapshot = scene;

  // copy to vector
  std::vector<char> bytes(json_str.begin(), json_str.end());

  FileSystem::writeFile(filepath, bytes);
  LOG_INFO << "Saved scene to: " << filepath;
}

json Scene::CreateSceneJSON(){
  json scene;
  json &entities = scene["entities"];

  size_t counter = 0;
  for (ecs::Entity entity : m_Entities) {
    std::string name = m_Coordinator.GetComponent<NameComponent>(entity).name;
    entities[name + std::to_string(counter)] = CreateJSONFromEntity(entity);
    counter++;
  }
  return scene;
}

void Scene::LoadScene(const char *filepath) {
  // load bytes
  std::vector<char> bytes;
  if (!FileSystem::loadFile(filepath, bytes)) {
    LOG_ERROR << "Could not load scene";
    return;
  }

  // parse json
  json j;
  try {
    j = json::parse(bytes);
  } catch (json::parse_error &e) {
    LOG_ERROR << "nlohmann::json::parse_error: \n"
              << e.byte << ": " << e.what();
    return;
  }
  
  LoadSceneJSON(j);
  mCurrentSceneSnapshot = CreateSceneJSON();

  LOG_INFO << "Loaded scene from: " << filepath;
}

void Scene::LoadSceneJSON(const json& j){
  ClearAllEntities();

  // error check
  if (!j.contains("entities")) {
    LOG_ERROR << "Could not find entities field";
    return;
  }

  // load all entities in json
  for (const auto &[e, c] : j["entities"].items()) {
    ecs::Entity entity = CreateEntityFromJSON(c);
  }
}

ecs::Entity Scene::CreateEntityFromJSON(const json &j) {
  ecs::Entity entity = m_Coordinator.CreateEntity();
  m_Coordinator.AddComponent<NameComponent>(entity, NameComponent{j["name"]});

  if (j.contains("transform_component"))
    m_Coordinator.AddComponent<TransformComponent>(entity,
                                                   j["transform_component"]);
  if (j.contains("render_component")) {
    m_Coordinator.AddComponent<RenderComponent>(entity, j["render_component"]);
    std::string meshType = j["render_component"].value("mesh_type", "");
    if (!meshType.empty()) {
      auto &rc = m_Coordinator.GetComponent<RenderComponent>(entity);
      if (meshType == "sphere") {
        std::vector<float> sv;
        std::vector<unsigned int> si;
        Sphere::generate_uvsphere(sv, si, 24, 16);
        rc.mesh = std::make_shared<Mesh>(sv, si);
      } else if (meshType == "cube") {
        std::vector<float> cv(Cube::s_Vertices,
                              Cube::s_Vertices + Cube::s_VertexCount * 8);
        std::vector<unsigned int> ci(Cube::s_Indices,
                                     Cube::s_Indices + Cube::s_IndexCount);
        rc.mesh = std::make_shared<Mesh>(cv, ci);
      } else if (meshType == "plane") {
        std::vector<float> pv(Plane::vertices,
                              Plane::vertices + PLANE_VERTEX_SIZE);
        std::vector<unsigned int> pi(Plane::indices,
                                     Plane::indices + PLANE_INDEX_SIZE);
        rc.mesh = std::make_shared<Mesh>(pv, pi);
      }
    }
  }
  if (j.contains("camera_component"))
    m_Coordinator.AddComponent<CameraComponent>(entity, j["camera_component"]);
  if (j.contains("pointLight_component"))
    m_Coordinator.AddComponent<PointLightComponent>(entity,
                                                    j["pointLight_component"]);
  if (j.contains("script_component"))
    m_Coordinator.AddComponent<ScriptComponent>(entity, j["script_component"]);
  if (j.contains("sphere_collider_component"))
    m_Coordinator.AddComponent<SphereColliderComponent>(
        entity, j["sphere_collider_component"]);
  if (j.contains("box_collider_component"))
    m_Coordinator.AddComponent<BoxColliderComponent>(
        entity, j["box_collider_component"]);
  if (j.contains("capsule_collider_component"))
    m_Coordinator.AddComponent<CapsuleColliderComponent>(
        entity, j["capsule_collider_component"]);
  if (j.contains("rigid_body_component"))
    m_Coordinator.AddComponent<RigidBodyComponent>(entity,
                                                   j["rigid_body_component"]);
  if(j.contains("terrain_component"))
    m_Coordinator.AddComponent<TerrainComponent>(entity, j["terrain_component"]);

  m_Entities.push_back(entity);
  return entity;
}

const json Scene::CreateJSONFromEntity(ecs::Entity entity) {
  json entity_object;
  std::string name = m_Coordinator.GetComponent<NameComponent>(entity).name;
  entity_object["name"] = name;

  // add components
  if (m_Coordinator.HasComponent<TransformComponent>(entity))
    entity_object["transform_component"] =
        m_Coordinator.GetComponent<TransformComponent>(entity);
  if (m_Coordinator.HasComponent<RenderComponent>(entity))
    entity_object["render_component"] =
        m_Coordinator.GetComponent<RenderComponent>(entity);
  if (m_Coordinator.HasComponent<CameraComponent>(entity))
    entity_object["camera_component"] =
        m_Coordinator.GetComponent<CameraComponent>(entity);
  if (m_Coordinator.HasComponent<PointLightComponent>(entity))
    entity_object["pointLight_component"] =
        m_Coordinator.GetComponent<PointLightComponent>(entity);
  if (m_Coordinator.HasComponent<ScriptComponent>(entity))
    entity_object["script_component"] =
        m_Coordinator.GetComponent<ScriptComponent>(entity);
  if (m_Coordinator.HasComponent<SphereColliderComponent>(entity))
    entity_object["sphere_collider_component"] =
        m_Coordinator.GetComponent<SphereColliderComponent>(entity);
  if (m_Coordinator.HasComponent<BoxColliderComponent>(entity))
    entity_object["box_collider_component"] =
        m_Coordinator.GetComponent<BoxColliderComponent>(entity);
  if (m_Coordinator.HasComponent<CapsuleColliderComponent>(entity))
    entity_object["capsule_collider_component"] =
        m_Coordinator.GetComponent<CapsuleColliderComponent>(entity);
  if (m_Coordinator.HasComponent<RigidBodyComponent>(entity))
    entity_object["rigid_body_component"] =
        m_Coordinator.GetComponent<RigidBodyComponent>(entity);
  if(m_Coordinator.HasComponent<TerrainComponent>(entity))
    entity_object["terrain_component"] = m_Coordinator.GetComponent<TerrainComponent>(entity);
  return entity_object;
}


void Scene::SetAspectRatio(float aspect) { mCameraSystem->SetAspect(aspect); }
