#pragma once

#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <array>
#include <memory>

#include "ecs_common.h"

class IComponentArray {
public:
  virtual ~IComponentArray() = default;
  virtual void EntityDestroyed(Entity entity) = 0;
};

template <typename T> class ComponentArray : public IComponentArray {
public:
  void InsertData(Entity entity, T component) {
    assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() &&
           "Component added to same entity more than once");
    // put a new entry at the end and update the maps
    size_t newIndex = mSize;
    mEntityToIndexMap[entity] = newIndex;
    mIndexToEntityMap[newIndex] = entity;
    mComponentArray[newIndex] = component;
    ++mSize;
  }
  void RemoveData(Entity entity) {
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() &&
           "Removing non-existant component");

    // copy element at end into deleted elements place to maintain density
    size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
    size_t indexOfLastElement = mSize - 1;
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    // update map to point to moved spot
    Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
    mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    mEntityToIndexMap.erase(entity);
    mIndexToEntityMap.erase(indexOfLastElement);

    --mSize;
  }

  T& GetData(Entity entity){
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() &&
           "Retrieving non-existing component");
    // retur a reference to the entity's component
    return mComponentArray[mEntityToIndexMap[entity]];
  }

  void EntityDestroyed(Entity entity) override{
    if(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()){
      RemoveData(entity);
    }
  }

private:
  std::array<T, MAX_ENTITIES> mComponentArray;
  std::unordered_map<Entity, size_t> mEntityToIndexMap;
  std::unordered_map<size_t, Entity> mIndexToEntityMap;
  size_t mSize;
};

