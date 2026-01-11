#pragma once

#include "Handle.h"
#include "HandleAllocator.h"
#include "Logger.h"
#include "ResourceManagerBase.h"
#include <iterator>
#include <unordered_map>

template <typename T> 
class ResourceManager : public ResourceManagerBase {
private:
  HandleAllocator<T> m_Allocator;
  std::unordered_map<Handle<T>, T*> m_Resources;

public:
  Handle<T> Create(T *resource) {
    LOG_INFO << "Creating resource";
    auto handle = m_Allocator.Allocate();
    m_Resources.emplace(handle, resource);
    return handle;
  }

  T *Get(Handle<T> handle) {
    if (!m_Allocator.IsValid(handle))
      return nullptr;

    auto it = m_Resources.find(handle);
    if (it == m_Resources.end())
      return nullptr;

    return it->second;
  }

  void Destroy(Handle<T> handle) {
    auto it = m_Resources.find(handle);
    if(it != m_Resources.end() && it->second){
      delete it->second;
      m_Resources.erase(it);
      m_Allocator.Free(handle);
    }
  }

  void DestroyAll() override {
    for(auto& [handle, resource] : m_Resources){
      if(resource) delete resource;
      m_Allocator.Free(handle);
    }
    m_Resources.clear();
  }

  auto &GetAll() { return m_Resources; }
  const auto &GetAll() const { return m_Resources; }
  const HandleAllocator<T> &GetAllocator() const { return m_Allocator; }
  const size_t GetResourceSize() const { return m_Resources.size(); }
};
