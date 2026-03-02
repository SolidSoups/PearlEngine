#include "PhysicsSystem.h"

#include "ecs_system_impl.h"
#include "LineRenderer.h"
#include "SelectionWizard.h"

void PhysicsSystem::UpdatePhysics(float timestep) {
  // integrate
  for (auto entity : Entities) {
    auto *rb = TryGet<RigidBodyComponent>(entity);
    if (!rb)
      continue;
    auto& tf = Get<TransformComponent>(entity);

    rb->velocity.y -= rb->gravity * timestep;
    tf.position += rb->velocity * timestep;
  }

  // broad phase pair checks
  for (auto it1 = Entities.begin(); it1 != Entities.end(); ++it1) {
    auto it2 = it1;
    ++it2;
    for (; it2 != Entities.end(); ++it2) {
      CheckCollision(*it1, *it2);
    }
  }
  
  FlushCollisions();
}

void PhysicsSystem::CheckCollision(ecs::Entity e1, ecs::Entity e2) {
  mCE1 = e1;
  mCE2 = e2;

  auto *rb1 = TryGet<RigidBodyComponent>(e1);
  auto *rb2 = TryGet<RigidBodyComponent>(e2);
  if (!rb1 and !rb2)
    return;

  auto &t1 = Get<TransformComponent>(e1);
  auto &t2 = Get<TransformComponent>(e2);

  auto *s1 = TryGet<SphereColliderComponent>(e1);
  auto *s2 = TryGet<SphereColliderComponent>(e2);
  auto *b1 = TryGet<BoxColliderComponent>(e1);
  auto *b2 = TryGet<BoxColliderComponent>(e2);
  auto *c1 = TryGet<CapsuleColliderComponent>(e1);
  auto *c2 = TryGet<CapsuleColliderComponent>(e2);

  if (s1 and s2)
    TestSphereSphere(t1, *s1, rb1, t2, *s2, rb2);

  if (s1 and b2)
    TestSphereBox(t1, *s1, rb1, t2, *b2, rb2);
  if (s2 and b1)
    TestSphereBox(t2, *s2, rb2, t1, *b1, rb1);

  if (s1 and c2)
    TestSphereCapsule(t1, *s1, rb1, t2, *c2, rb2);
  if (s2 and c1)
    TestSphereCapsule(t2, *s2, rb2, t1, *c1, rb1);

  if (b1 and b2)
    TestBoxBox(t1, *b1, rb1, t2, *b2, rb2);

  if (b1 and c2)
    TestBoxCapsule(t1, *b1, rb1, t2, *c2, rb2);
  if (b2 and c1)
    TestBoxCapsule(t2, *b2, rb2, t1, *c1, rb1);

  if (c1 and c2)
    TestCapsuleCapsule(t1, *c1, rb1, t2, *c2, rb2);
}

// Normal needs to point A->B
void PhysicsSystem::Resolve(TransformComponent &tf1, RigidBodyComponent *rb1,
             TransformComponent &tf2, RigidBodyComponent *rb2,
             const glm::vec3 &normal, float penetration) {
  // position correction
  if (rb1 and rb2) {
    tf1.position -= normal * (penetration * 0.5f);
    tf2.position += normal * (penetration * 0.5f);
  } else if (rb1) {
    tf1.position -= normal * penetration;
  } else {
    tf2.position += normal * penetration;
  }

  // velocity impulse
  glm::vec3 v1 = rb1 ? rb1->velocity : glm::vec3(0);
  glm::vec3 v2 = rb2 ? rb2->velocity : glm::vec3(0);
  float vAlongNormal = glm::dot(v1 - v2, normal);
  if (vAlongNormal < 0)
    return; // already seperating

  float restitution = 0.4f;
  float invM1 = rb1 ? 1.0f / rb1->mass : 0.0f;
  float invM2 = rb2 ? 1.0f / rb2->mass : 0.0f;
  float j = -(1.0f + restitution) * vAlongNormal / (invM1 + invM2);

  if (rb1)
    rb1->velocity += j * invM1 * normal;
  if (rb2)
    rb2->velocity -= j * invM2 * normal;

  RegisterCollision(mCE1, mCE2, normal, penetration);
}

void PhysicsSystem::TestSphereSphere(TransformComponent &tf1,
                                     SphereColliderComponent &s1,
                                     RigidBodyComponent *rb1,
                                     TransformComponent &tf2,
                                     SphereColliderComponent &s2,
                                     RigidBodyComponent *rb2) {
  glm::vec3 posA = tf1.position + s1.position;
  glm::vec3 posB = tf2.position + s2.position;
  glm::vec3 diff = posB - posA;
  float dist = glm::length(diff);
  float overlap = s1.radius + s2.radius - dist;
  if (overlap <= 0)
    return; // no overlap

  glm::vec3 normal = dist > 0.0001f ? diff / dist : glm::vec3(0, 1, 0);
  Resolve(tf1, rb1, tf2, rb2, normal, overlap);
}

void PhysicsSystem::TestSphereBox(TransformComponent &tf1,
                                  SphereColliderComponent &s1,
                                  RigidBodyComponent *rb1,
                                  TransformComponent &tf2,
                                  BoxColliderComponent &b2,
                                  RigidBodyComponent *rb2) {
  glm::vec3 spherePos = tf1.position + s1.position;
  glm::vec3 boxCenter = tf2.position + b2.center;
  glm::vec3 half = b2.size * 0.5f;

  glm::vec3 closest = glm::clamp(spherePos, boxCenter - half, boxCenter + half);
  glm::vec3 diff = spherePos - closest;
  float dist = glm::length(diff);
  if (dist >= s1.radius)
    return;

  float overlap = s1.radius - dist;
  glm::vec3 normal = dist > 0.0001f ? -diff / dist : glm::vec3(0, -1, 0);
  Resolve(tf1, rb1, tf2, rb2, normal, overlap);
}
void PhysicsSystem::TestSphereCapsule(TransformComponent &tf1,
                                      SphereColliderComponent &s1,
                                      RigidBodyComponent *rb1,
                                      TransformComponent &tf2,
                                      CapsuleColliderComponent &c2,
                                      RigidBodyComponent *rb2) {
  glm::vec3 spherePos = tf1.position + s1.position;
  glm::vec3 a = tf2.position + c2.a;
  glm::vec3 b = tf2.position + c2.b;

  // closest point on capsule segment to sphere center
  glm::vec3 ab = b - a;
  float t =
      glm::clamp(glm::dot(spherePos - a, ab) / glm::dot(ab, ab), 0.0f, 1.0f);
  glm::vec3 closest = a + t * ab;

  glm::vec3 diff = spherePos - closest;
  float dist = glm::length(diff);
  float combinedR = s1.radius + c2.radius;
  if (dist >= combinedR)
    return;

  float overlap = combinedR - dist;
  glm::vec3 normal = dist > 0.0001f ? -diff / dist : glm::vec3(0, -1, 0);
  Resolve(tf1, rb1, tf2, rb2, normal, overlap);
}
void PhysicsSystem::TestBoxBox(TransformComponent &tf1,
                               BoxColliderComponent &b1,
                               RigidBodyComponent *rb1, TransformComponent &tf2,
                               BoxColliderComponent &b2,
                               RigidBodyComponent *rb2) {
  glm::vec3 c1 = tf1.position + b1.center, h1 = b1.size * 0.5f;
  glm::vec3 c2 = tf2.position + b2.center, h2 = b2.size * 0.5f;
  glm::vec3 diff = c2 - c1;
  glm::vec3 overlap = (h1 + h2) - glm::abs(diff);
  if (overlap.x <= 0 || overlap.y <= 0 || overlap.z <= 0)
    return;

  // pick axis with smallest overlap = contact normal
  glm::vec3 normal;
  float pen;
  if (overlap.x < overlap.y && overlap.x < overlap.z) {
    pen = overlap.x;
    normal = {diff.x > 0 ? 1.0f : -1.0f, 0, 0};
  } else if (overlap.y < overlap.z) {
    pen = overlap.y;
    normal = {0, diff.y > 0 ? 1.0f : -1.0f, 0};
  } else {
    pen = overlap.z;
    normal = {0, 0, diff.z > 0 ? 1.0f : -1.0f};
  }
  Resolve(tf1, rb1, tf2, rb2, normal, pen);
}
void PhysicsSystem::TestBoxCapsule(TransformComponent &t1,
                                   BoxColliderComponent &sp1,
                                   RigidBodyComponent *rb1,
                                   TransformComponent &t2,
                                   CapsuleColliderComponent &sp2,
                                   RigidBodyComponent *rb2) {}
void PhysicsSystem::TestCapsuleCapsule(TransformComponent &tf1,
                                       CapsuleColliderComponent &c1,
                                       RigidBodyComponent *rb1,
                                       TransformComponent &tf2,
                                       CapsuleColliderComponent &c2,
                                       RigidBodyComponent *rb2) {
  glm::vec3 a1 = tf1.position + c1.a, b1 = tf1.position + c1.b;
  glm::vec3 a2 = tf2.position + c2.a, b2 = tf2.position + c2.b;

  // closest point between two segments (Ericson, Real-Time Collision Detection
  // p.149)
  glm::vec3 d1 = b1 - a1, d2 = b2 - a2, r = a1 - a2;
  float e = glm::dot(d2, d2), f = glm::dot(d2, r);
  float s, t;
  float a = glm::dot(d1, d1), c = glm::dot(d1, r);
  float b = glm::dot(d1, d2);
  float denom = a * e - b * b;
  s = denom > 0.0001f ? glm::clamp((b * f - c * e) / denom, 0.0f, 1.0f) : 0.0f;
  t = (b * s + f) / e;
  if (t < 0) {
    t = 0;
    s = glm::clamp(-c / a, 0.0f, 1.0f);
  } else if (t > 1) {
    t = 1;
    s = glm::clamp((b - c) / a, 0.0f, 1.0f);
  }

  glm::vec3 p1 = a1 + s * d1;
  glm::vec3 p2 = a2 + t * d2;

  glm::vec3 diff = p1 - p2;
  float dist = glm::length(diff);
  float combinedR = c1.radius + c2.radius;
  if (dist >= combinedR)
    return;

  float overlap = combinedR - dist;
  glm::vec3 normal = dist > 0.0001f ? -diff / dist : glm::vec3(0, -1, 0);
  Resolve(tf1, rb1, tf2, rb2, normal, overlap);
}

void PhysicsSystem::DrawGizmos() {
  ecs::Entity selectedEntity = SelectionWizard::Get();
  if (selectedEntity == ecs::NULL_ENTITY)
    return;

  SphereColliderComponent sphereCmp;
  BoxColliderComponent boxComp;
  for (ecs::Entity entity : Entities) {
    if (entity != selectedEntity)
      continue;
    auto &transform = Get<TransformComponent>(entity);

    if (TryGet<SphereColliderComponent>(entity, sphereCmp)) {
      LineRenderer::DrawWireSphere(transform.position + sphereCmp.position,
                                   sphereCmp.radius);
    }
    if (TryGet<BoxColliderComponent>(entity, boxComp)) {
      LineRenderer::DrawWireBox(transform.position + boxComp.center,
                                boxComp.size);
    }
    if (auto *capsule = TryGet<CapsuleColliderComponent>(entity)) {
      LineRenderer::DrawWireCapsule(transform.position + capsule->a,
                                    transform.position + capsule->b,
                                    capsule->radius);
    }
  }
}


void PhysicsSystem::FlushCollisions(){
  for(auto [key, pair] : mCurrentCollisionPairs){
    if(mPrevCollisions.count(key)){
      // OnCollisionStay
    }
    else{
      // OnCollisionEnter
      const glm::vec3& normal = pair.first;
      float penetration = pair.second;
      mScriptSystem->DispatchOnCollisionEnter(mCE1, mCE2, normal, penetration);
    }
  }
  for(auto [key, pair] : mPrevCollisions){
    if(!mCurrentCollisionPairs.count(key)){
      // OnCollisionExit 
    }
  }

  mPrevCollisions = std::move(mCurrentCollisionPairs);
  mCurrentCollisionPairs.clear();
}

void PhysicsSystem::RegisterCollision(ecs::Entity a, ecs::Entity b, const glm::vec3& normal, float pen){
  // ensure collision pairs don't have any hash collisions in unorderd_map
  // by always setting lowest entity id first
  uint64_t lo = std::min(a, b);
  uint64_t high = std::max(a, b);
  mCurrentCollisionPairs[(high << 32) | lo] = std::make_pair(normal, pen);
}
