#pragma once

#include <memory>
#include <iostream>

#include "ecs_common.h"
#include "ecs_system.h"

namespace ecs {

class SystemManager {
public:
  template <typename T> std::shared_ptr<T> RegisterSystem() {
    const char *typeName = typeid(T).name();

    assert(mSystems.find(typeName) == mSystems.end() &&
           "Registering system more than once.");

    // create a pointer to the system and return it 
    auto system = std::make_shared<T>();
    mSystems.insert({typeName, system});
    return system;
  }

  template <typename T> void SetSignature(Signature signature) {
    const char *typeName = typeid(T).name();

    assert(mSystems.find(typeName) != mSystems.end() &&
           "System used before registered.");

    // set the signature for this system
    mSignatures.insert({typeName, signature});
  }

  void EntityDestroyed(Entity entity) {
    // erase a destroyed entity from all lists
    for (auto const &pair : mSystems) {
      auto const &system = pair.second;

      system->mEntities.erase(entity);
    }
  }

  void EntitySignatureChanged(Entity entity, Signature entitySignature) {
    // notify each system that an entity's signature changed
    for (auto const &pair : mSystems) {
      auto const &type = pair.first;
      auto const &system = pair.second;

      auto sig_it = mSignatures.find(type);
      if(sig_it == mSignatures.end()) {
        continue;
      }
      auto const &systemSignature = sig_it->second;

      // if entity signature matches, insert the entity into the systems map
      if ((entitySignature & systemSignature) == systemSignature) {
        system->mEntities.insert(entity);
      }
      // if entity signature does not match, erase the entity from the systems map
      // (if it exists)
      else {
        system->mEntities.erase(entity);
      }
    }
  }

private:
  std::unordered_map<std::string, Signature> mSignatures{};
  std::unordered_map<std::string, std::shared_ptr<System>> mSystems{};
};
} // namespace ecs
