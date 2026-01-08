#pragma once

#include "Handle.h"
#include "HandleAllocator.h"
#include "IResource.h"
#include "Logger.h"
#include "ResourceManagerBase.h"
#include <iterator>
#include <unordered_map>
#include "ResourceTraits.h"

template <typename Tag> class ResourceManager : public ResourceManagerBase {
public:
  using Data = typename TagToData<Tag>::Data;

private:
  HandleAllocator<Tag> m_Allocator;
  std::unordered_map<Handle<Tag>, IResource *> m_Resources;

public:
  Handle<Tag> Create(IResource *resource) {
    LOG_INFO << "Creating resource";
    auto handle = m_Allocator.Allocate();
    m_Resources.emplace(handle, resource);
    return handle;
  }

  Data *Get(Handle<Tag> handle) {
    if (!m_Allocator.IsValid(handle))
      return nullptr;

    // find resource
    auto it = m_Resources.find(handle);
    if (it == m_Resources.end())
      return nullptr;

    // downcast datatype
    IResource *resourceData = it->second;
    Data *data = dynamic_cast<Data *>(resourceData);
    return data;
  }

  void Destroy(Handle<Tag> handle) {
    if (m_Resources[handle])
      delete m_Resources[handle];

    m_Resources.erase(handle);
    m_Allocator.Free(handle);
  }

  // this might work for now. TODO: investigate if we need manual cleanup or
  // cleanup of handle allocator
  void DestroyAll() override { m_Resources.clear(); }

  auto &GetAll() { return m_Resources; }
  const auto &GetAll() const { return m_Resources; }

  const HandleAllocator<Tag> &GetAllocator() const { return m_Allocator; }
  const size_t GetResourceSize() const { return m_Resources.size(); }
};
