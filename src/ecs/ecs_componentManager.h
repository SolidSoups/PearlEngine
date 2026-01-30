#pragma once

#include <cassert>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "ecs_componentArray.h"

namespace ecs {

class ComponentManager {
public:
  template<typename T>
  void RegisterComponent(){
    const char* typeName = typeid(T).name();
    assert(mComponentTypes.find(typeName) == mComponentTypes.end()
           && "Registering component more than once");

    mComponentTypes.insert({typeName, mNextComponentType});
    mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
  }

  template<typename T>
  ComponentType GetComponentType(){

    const char* typeName = typeid(T).name();
    assert(mComponentTypes.find(typeName) != mComponentTypes.end() 
           && "Component not registered");

    return mComponentTypes[typeName];
  }

  template<typename T>
  void AddComponent(Entity entity, T component){
    GetComponentArray<T>()->InsertData(entity, component);
  }

  template<typename T>
  void RemoveComponent(Entity entity) {
    GetComponentArray<T>()->RemoveData(entity);
  }

  template<typename T>
  T& GetComponent(Entity entity){
    return GetComponentArray<T>()->GetData(entity);
  }

  template<typename T>
  bool HasComponent(Entity entity){
    return GetComponentArray<T>()->HasEntity(entity);
  }

  void EntityDestroyed(Entity entity){
    // notify each component array that an entity as been destroyed
    for(const auto& pair : mComponentArrays){
      auto const& componentArray = pair.second;
      componentArray->EntityDestroyed(entity);
    }
  }

private:
  std::unordered_map<const char*, ComponentType> mComponentTypes{};
  std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
  ComponentType mNextComponentType{};

  template<typename T>
  std::shared_ptr<ComponentArray<T>> GetComponentArray(){
    const char* typeName = typeid(T).name();
    assert(mComponentTypes.find(typeName) != mComponentTypes.end() &&
           "Component not registered before use");
    return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
  }
};
}
