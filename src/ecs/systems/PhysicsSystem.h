#pragma once

#include <glm/gtc/quaternion.hpp>

#include "ecs_system.h"

#include "TransformComponent.h"
#include "SphereColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "BoxColliderComponent.h"
#include "RigidBodyComponent.h"
#include "ScriptSystem.h"
#include "TerrainComponent.h"
#include "TerrainSystem.h"


struct CollisionEvent {
  ecs::Entity me, other;
  glm::vec3 normal; //  me -> other
  float penetration;
};

class PhysicsSystem : public ecs::System {
private:
  ScriptSystem* mScriptSystem;
  TerrainSystem* mTerrainSystem;

  std::unordered_map<uint64_t, CollisionEvent> mPrevCollisions;
  std::unordered_map<uint64_t, CollisionEvent> mCurrentCollisionPairs;
public:
  inline void Init(ScriptSystem* scriptSystem, TerrainSystem* terrainSystem){
    mScriptSystem = scriptSystem;
    mTerrainSystem = terrainSystem;
  }
  void UpdatePhysics(float timestep);
  void DrawGizmos();

private:
  void CheckCollision(ecs::Entity a, ecs::Entity b);
  void TestSphereSphere(TransformComponent &t1, SphereColliderComponent &sp1,
                        RigidBodyComponent *rb1, TransformComponent &t2,
                        SphereColliderComponent &sp2, RigidBodyComponent *rb2,
                        ecs::Entity a, ecs::Entity b);
  void TestSphereBox(TransformComponent &t1, SphereColliderComponent &sp1,
                     RigidBodyComponent *rb1, TransformComponent &t2,
                     BoxColliderComponent &sp2, RigidBodyComponent *rb2,
                     ecs::Entity a, ecs::Entity b);
  void TestSphereCapsule(TransformComponent &t1, SphereColliderComponent &sp1,
                         RigidBodyComponent *rb1, TransformComponent &t2,
                         CapsuleColliderComponent &sp2,
                         RigidBodyComponent *rb2,
                         ecs::Entity a, ecs::Entity b);
  void TestBoxBox(TransformComponent &t1, BoxColliderComponent &sp1,
                  RigidBodyComponent *rb1, TransformComponent &t2,
                  BoxColliderComponent &sp2, RigidBodyComponent *rb2,
                  ecs::Entity a, ecs::Entity b);
  void TestBoxCapsule(TransformComponent &t1, BoxColliderComponent &sp1,
                      RigidBodyComponent *rb1, TransformComponent &t2,
                      CapsuleColliderComponent &sp2, RigidBodyComponent *rb2,
                      ecs::Entity a, ecs::Entity b);
  void TestCapsuleCapsule(TransformComponent &t1, CapsuleColliderComponent &sp1,
                          RigidBodyComponent *rb1, TransformComponent &t2,
                          CapsuleColliderComponent &sp2,
                          RigidBodyComponent *rb2,
                          ecs::Entity a, ecs::Entity b);
  void TestSphereTerrain(
    TransformComponent & sphereTf, SphereColliderComponent& sphere,
    RigidBodyComponent *rb, ecs::Entity sphereEntity,
    TransformComponent & terrainTf, TerrainComponent& terrain,
    ecs::Entity terrainEntity
  );
  void Resolve(TransformComponent& tf1, RigidBodyComponent* rb1,
               TransformComponent& tf2, RigidBodyComponent* rb2,
               const glm::vec3& normal, float penetration,
               ecs::Entity a, ecs::Entity b);

  void RegisterCollision(ecs::Entity a, ecs::Entity b, const glm::vec3& normal, float pen);
  void FlushCollisions();

  static glm::quat RotQuat(const TransformComponent& tf) {
    glm::mat4 m(1.0f);
    m = glm::rotate(m, glm::radians(tf.rotation.x), {1,0,0});
    m = glm::rotate(m, glm::radians(tf.rotation.y), {0,1,0});
    m = glm::rotate(m, glm::radians(tf.rotation.z), {0,0,1});
    return glm::quat_cast(m);
  }
};
