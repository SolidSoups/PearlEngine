#pragma once

#include "Handle.h"
#include "HandleAllocator.h"
#include "ResourceManagerBase.h"
#include <unordered_map>

template <typename Tag, typename Data>
class ResourceManager : public ResourceManagerBase {
    HandleAllocator<Tag> m_Allocator;
    std::unordered_map<Handle<Tag>, Data> m_Resources;

  public:
    Handle<Tag> Create(Data data) {
        auto handle = m_Allocator.Allocate();
        m_Resources[handle] = std::move(data);
        return handle;
    }

    Data *Get(Handle<Tag> handle) {
        if (!m_Allocator.IsValid(handle))
            return nullptr;
        auto it = m_Resources.find(handle);
        return it != m_Resources.end() ? &it->second : nullptr;
    }

    void Destroy(Handle<Tag> handle) {
        m_Resources.erase(handle);
        m_Allocator.Free(handle);
    }

    // this might work for now. TODO: investigate if we need manual cleanup or cleanup of handle allocator
    void DestroyAll() override { m_Resources.clear(); }

    auto &GetAll() { return m_Resources; }
    const auto &GetAll() const { return m_Resources; }

    const HandleAllocator<Tag> &GetAllocator() const { return m_Allocator; }
    const size_t GetResourceSize() const { return m_Resources.size(); }
};
