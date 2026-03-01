#pragma once

#include <memory>
#include <iostream>

#include "ecs_common.h"
#include "ecs_system.h"

namespace ecs {

class SystemManager {
public:
  // register a system
  template <typename T> std::shared_ptr<T> RegisterSystem() {
    const char *typeName = typeid(T).name();

    assert(mSystems.find(typeName) == mSystems.end() &&
           "Registering system more than once.");

    // create a pointer to the system and return it 
    auto system = std::make_shared<T>();
    mSystems.insert({typeName, system});
    return system;
  }

  // Set the signature (what components it needs)
  template <typename T> void SetSignature(Signature signature) {
    const char *typeName = typeid(T).name();

    assert(mSystems.find(typeName) != mSystems.end() &&
           "System used before registered.");

    // set the signature for this system
    mSignatures.insert({typeName, signature});
  }

  // Set interest (OR) signature — entity qualifies if it has ANY of these bits
  template <typename T> void SetInterestSignature(Signature signature) {
    const char *typeName = typeid(T).name();
    assert(mSystems.find(typeName) != mSystems.end() &&
           "System used before registered.");
    mInterestSignatures.insert({typeName, signature});
  }

  // Erase an entity 
  void EntityDestroyed(Entity entity) {
    // erase a destroyed entity from all lists
    for (auto const &pair : mSystems) {
      auto const &system = pair.second;

      system->Entities.erase(entity);
    }
  }

  // broadcast that a signature has changed
  void EntitySignatureChanged(Entity entity, Signature entitySignature) {
    for (auto const &pair : mSystems) {
      auto const &type   = pair.first;
      auto const &system = pair.second;

      auto req_it = mSignatures.find(type);
      auto int_it = mInterestSignatures.find(type);

      // system has no signatures registered — skip (no auto-population)
      if (req_it == mSignatures.end() && int_it == mInterestSignatures.end()) {
        continue;
      }

      bool passesRequired = (req_it == mSignatures.end()) ||
                            ((entitySignature & req_it->second) == req_it->second);
      bool passesInterest = (int_it == mInterestSignatures.end()) ||
                            (entitySignature & int_it->second).any();

      if (passesRequired && passesInterest) {
        system->Entities.insert(entity);
      } else {
        system->Entities.erase(entity);
      }
    }
  }

private:
  std::unordered_map<std::string, Signature> mSignatures{};
  std::unordered_map<std::string, Signature> mInterestSignatures{};
  std::unordered_map<std::string, std::shared_ptr<System>> mSystems{};
};
} // namespace ecs
