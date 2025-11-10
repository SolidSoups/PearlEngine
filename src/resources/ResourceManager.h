#pragma once

#include "HandleAllocator.h"
#include "Handle.h"
#include <unordered_map>

template<typename Tag, typename Data>
class ResourceManager {
  HandleAllocator<Tag> m_Allocator;
  std::unordered_map<Handle<Tag>, Data> m_Resources; 

public:
  Handle<Tag> Create(Data data){
    auto handle = m_Allocator.Allocate();
    m_Resources[handle] = std::move(data);
    return handle;
  }

  Data* Get(Handle<Tag> handle){
    if(!m_Allocator.IsValid(handle)) return nullptr;
    auto it = m_Resources.find(handle);
    return it != m_Resources.end() ? &it->second : nullptr;
  }

  void Destroy(Handle<Tag> handle){
    m_Resources.erase(handle); // TODO: slooooow
    m_Allocator.Free(handle);
  }
};
