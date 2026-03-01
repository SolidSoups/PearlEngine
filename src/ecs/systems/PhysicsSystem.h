#pragma once

#include "ecs_system.h"

#include "TransformComponent.h"
#include "SphereColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "BoxColliderComponent.h"
#include "RigidBodyComponent.h"

class PhysicsSystem : public ecs::System {
public:
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
};
