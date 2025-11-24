#pragma once

#include <algorithm>
#include <memory>
#include <typeindex>
#include <vector>

#include "ResourceTraits.h"
#include "MaterialData.h"
#include "MeshData.h"
#include "ResourceManager.h"
#include "ShaderData.h"
#include "TextureData.h"

#include "Logger.h"

class ResourceSystem {
    std::unordered_map<std::type_index, std::unique_ptr<ResourceManagerBase>>
        m_Managers;

public:
    // ResourceSystem(const ResourceSystem&) = delete; //idkw this doesn't work
    ResourceSystem &operator=(const ResourceSystem&) = delete;

  public:
    static ResourceSystem &Get() {
        static ResourceSystem instance;
        return instance;
    }

    // Create only a specific data type
    template<typename Data>
    Handle<typename ResourceTraits<Data>::Tag> Create(Data data){
        // basically a "trust me bro (compiler), it exists and is a type" for ::Tag
        using Tag = typename ResourceTraits<Data>::Tag; 
        return GetManager<Tag, Data>().Create(std::move(data));
    };

    // Get, extract a data type from Tag in handle
    template<typename Tag>
    typename TagToData<Tag>::Data* Get(Handle<Tag> handle){
        using Data = typename TagToData<Tag>::Data;
        return GetManager<Tag, Data>().Get(handle);
    }

    // Destroy
    template<typename Tag>
    void Destroy(Handle<Tag> handle){
        using Data = typename TagToData<Tag>::Data;
        return GetManager<Tag, Data>().Destroy(handle);
    }

    template<typename Tag, typename Data>
    ResourceManager<Tag, Data>& GetManager(){
        auto typeIdx = std::type_index(typeid(Tag));

        if(m_Managers.find(typeIdx) == m_Managers.end()){
            m_Managers[typeIdx] = std::make_unique<ResourceManager<Tag, Data>>();
        }

        return *static_cast<ResourceManager<Tag, Data>*>(m_Managers[typeIdx].get());
    }

    template<typename Data>
    auto& GetManagerByData(){
        using Tag = typename ResourceTraits<Data>::Tag;
        return GetManager<Tag, Data>();
    }



    void DestroyAllResources() {
        for(auto& [typeIdx, manager] : m_Managers){
            if(manager){
                manager->DestroyAll();
            }
        }
        m_Managers.clear();
    }
};
