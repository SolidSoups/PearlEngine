#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <typeindex>
#include <vector>

#include "IResource.h"
#include "IResourceConverter.h"
#include "Logger.h"
#include "MaterialData.h"
#include "ResourceManager.h"
#include "ResourceTraits.h"
#include "ShaderData.h"
#include "StaticRegistry.h"
#include "TextureData.h"

class ResourceSystem {
public:
  using ResourceConverters =
      StaticRegistry<std::type_index, IResourceToAssetConverter>;
  ResourceConverters AssetConverters;

private:
  using ResourceManagers =
      std::unordered_map<std::type_index, std::unique_ptr<ResourceManagerBase>>;
  ResourceManagers m_Managers;

public:
  // ResourceSystem(const ResourceSystem&) = delete; //idkw this doesn't work
  ResourceSystem &operator=(const ResourceSystem &) = delete;

public:
  IResource *LoadAsset(IAsset *asset) {
    auto *converter = AssetConverters.Get(asset->GetTypeIndex());
    if (!converter) {
      LOG_ERROR << "NO CONVERTER FOR THIS ASSET TYPE '"
                << asset->GetTypeIndex().name() << "'";
      return nullptr;
    }
    return converter->ConvertToResource(asset);
  }

  template <typename Data>
  Handle<typename ResourceTraits<Data>::Tag>
  LoadAsset(typename DataToAsset<Data>::Asset *asset) {
    using AssetType = typename DataToAsset<Data>::Asset;
    using Tag = typename ResourceTraits<Data>::Tag;
    using HandleType = Handle<typename ResourceTraits<Data>::Tag>;
    using RManager = ResourceManager<Tag>;

    // check if we have a converter for this type of asset
    auto *converter = AssetConverters.Get(std::type_index(typeid(AssetType)));
    if (!converter) {
      LOG_ERROR << "No asset to resource converter for this type: "
                << std::type_index(typeid(AssetType)).name();
      return HandleType::Invalid();
    }

    IResource *resource = converter->ConvertToResource(asset);
    if (!resource) {
      LOG_ERROR << "Converter failed to create resource";
      return HandleType::Invalid();
    }

    // downcast resource type
    Data* typedResource = dynamic_cast<Data*>(resource);
    if(!typedResource){
      LOG_ERROR << "Failed to downcast IResource to specific type";
      return HandleType::Invalid();
    }

    return Create<Data>(typedResource);
  }

  // Create only a specific data type
  template <typename Data>
  Handle<typename ResourceTraits<Data>::Tag> Create(Data *data) {
    using Tag = typename ResourceTraits<Data>::Tag;

    auto *resourceCast = dynamic_cast<IResource *>(data);
    if (!resourceCast) {
      LOG_ERROR << "CANNOT CAST '*data' INTO '*IResource'. "
                << "'data' argument must be derive from 'IResource'";
      return Handle<Tag>::Invalid();
    }
    Handle<Tag> handle = GetManager<Tag>().Create(data);
    if(handle == Handle<Tag>::Invalid()){
      LOG_ERROR << "Handle is invalid!";
    }
    return handle;
  };

  // Get, extract a data type from Tag in handle
  template <typename Tag>
  typename TagToData<Tag>::Data *Get(Handle<Tag> handle) {
    using Data = typename TagToData<Tag>::Data;
    return GetManager<Tag>().Get(handle);
  }

  // Destroy
  template <typename Tag> void Destroy(Handle<Tag> handle) {
    using Data = typename TagToData<Tag>::Data;
    return GetManager<Tag>().Destroy(handle);
  }

  template <typename Tag> ResourceManager<Tag> &GetManager() {
    auto typeIdx = std::type_index(typeid(Tag));

    if (m_Managers.find(typeIdx) == m_Managers.end()) {
      m_Managers[typeIdx] = std::make_unique<ResourceManager<Tag>>();
    }

    return *static_cast<ResourceManager<Tag> *>(m_Managers[typeIdx].get());
  }

  template <typename Tag> bool HasManagerForTag() {
    auto typeIdx = std::type_index(typeid(Tag));
    if (m_Managers.find(typeIdx) == m_Managers.end()) {
      return false;
    }
    return true;
  }

  template <typename Data> auto &GetManagerByData() {
    using Tag = typename ResourceTraits<Data>::Tag;
    return GetManager<Tag>();
  }

  void DestroyAllResources() {
    for (auto &[typeIdx, manager] : m_Managers) {
      if (manager) {
        manager->DestroyAll();
      }
    }
    m_Managers.clear();
  }
};
