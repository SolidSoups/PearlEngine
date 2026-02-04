#pragma once

#include <cstdint>
#include <bitset>
#include <cassert>
#include <queue>
#include <array>

#include <glm/glm.hpp>

#include "Logger.h"
#include "ecs_common.h"

// manager
namespace ecs {

class EntityManager {
public:
  EntityManager() {
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
      mAvailableEntities.push(entity);
    }
  }

  Entity CreateEntity() {
    assert(mLivingEntityCount < MAX_ENTITIES &&
           "Too many entities in existance");

    // take an id from the top of the queue
    Entity id = mAvailableEntities.front();
    mAvailableEntities.pop();
    ++mLivingEntityCount;

    return id;
  }

  void DestroyEntity(Entity entity) {
    assert(entity < MAX_ENTITIES && "Entity out of range");

    // invalidate the destroyed entity's signature
    mSignatures[entity].reset();

    // put the destroyed id at the back of the queue
    mAvailableEntities.push(entity);
    --mLivingEntityCount;
  }

  void SetSignature(Entity entity, Signature signature) {
    assert(entity < MAX_ENTITIES && "Entity out of range");

    // put this entity's signature into the array
    mSignatures[entity] = signature;
  }

  const Signature& GetSignature(Entity entity) {
    assert(entity < MAX_ENTITIES && "Entity out of range");

    return mSignatures[entity];
  }

private:
  std::queue<Entity> mAvailableEntities{};
  std::array<Signature, MAX_ENTITIES> mSignatures{};
  uint32_t mLivingEntityCount{};
};
} // namespace ecs
