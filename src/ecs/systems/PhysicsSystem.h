#pragma once

#include "ecs_system.h"

#include "TransformComponent.h"
#include "SphereColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "BoxColliderComponent.h"
#include "RigidBodyComponent.h"
#include "ScriptSystem.h"

struct CollisionEvent {
  ecs::Entity me, other;
  glm::vec3 normal; //  me -> other
  float penetration;
};

class PhysicsSystem : public ecs::System {
private:
  ScriptSystem* mScriptSystem;
  // keep track of the last entities a collision check was made for
  ecs::Entity mCE1 = ecs::NULL_ENTITY;
  ecs::Entity mCE2 = ecs::NULL_ENTITY;

  std::unordered_map<uint64_t, std::pair<glm::vec3, float>> mPrevCollisions;
  std::unordered_map<uint64_t, std::pair<glm::vec3, float>> mCurrentCollisionPairs;
public:
  inline void Init(ScriptSystem* scriptSystem){
    mScriptSystem = scriptSystem;
  }
  void UpdatePhysics(float timestep);
  void DrawGizmos();

private:
  void CheckCollision(ecs::Entity a, ecs::Entity b);
  void TestSphereSphere(TransformComponent &t1, SphereColliderComponent &sp1,
                        RigidBodyComponent *rb1, TransformComponent &t2,
                        SphereColliderComponent &sp2, RigidBodyComponent *rb2);
  void TestSphereBox(TransformComponent &t1, SphereColliderComponent &sp1,
                     RigidBodyComponent *rb1, TransformComponent &t2,
                     BoxColliderComponent &sp2, RigidBodyComponent *rb2);
  void TestSphereCapsule(TransformComponent &t1, SphereColliderComponent &sp1,
                         RigidBodyComponent *rb1, TransformComponent &t2,
                         CapsuleColliderComponent &sp2,
                         RigidBodyComponent *rb2);
  void TestBoxBox(TransformComponent &t1, BoxColliderComponent &sp1,
                  RigidBodyComponent *rb1, TransformComponent &t2,
                  BoxColliderComponent &sp2, RigidBodyComponent *rb2);
  void TestBoxCapsule(TransformComponent &t1, BoxColliderComponent &sp1,
                      RigidBodyComponent *rb1, TransformComponent &t2,
                      CapsuleColliderComponent &sp2, RigidBodyComponent *rb2);
  void TestCapsuleCapsule(TransformComponent &t1, CapsuleColliderComponent &sp1,
                          RigidBodyComponent *rb1, TransformComponent &t2,
                          CapsuleColliderComponent &sp2,
                          RigidBodyComponent *rb2);
  void Resolve(TransformComponent& tf1, RigidBodyComponent* rb1,
               TransformComponent& tf2, RigidBodyComponent* rb2,
               const glm::vec3& normal, float penetration);

  void RegisterCollision(ecs::Entity a, ecs::Entity b, const glm::vec3& normal, float pen);
  void FlushCollisions();
};
