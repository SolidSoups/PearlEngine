#pragma once


#include <string>
#include <unordered_map>
#include <optional>
#include <typeindex>

#include "AssetLoader.h"
#include "Logger.h"

namespace pe{
class Assets{
private:
    struct LoaderEntry{
        std::unique_ptr<IAssetLoader> loader;
        std::type_index assetType;
    };

public:
    static Assets& Get(){
        static Assets instance;
        return instance;
    }

    // Register any loader of type IAssetLoader
    template<typename AssetType>
    void RegisterLoader(const std::string& extension, std::unique_ptr<AssetLoader<AssetType>> loader){
        m_Loaders.insert_or_assign(extension, LoaderEntry{
            std::move(loader),
            std::type_index(typeid(AssetType))
        });
        LOG_INFO
            << "Registered loader for extension '" << extension << "' "
            << "and asset type" << typeid(AssetType).name();
    }

    // Get the loader and cast to a specific type
    template<typename AssetType>
    AssetLoader<AssetType>* GetLoader(const std::string& extension){
        auto it = m_Loaders.find(extension);
        if(it == m_Loaders.end()){
            LOG_ERROR << "No loader exists for file extension " << extension;
            return nullptr;
        }

        // check if type matches
        if(it->second.assetType != std::type_index(typeid(AssetType))){
            LOG_ERROR 
                << "Type mismatch! Extension '" << extension
                << "' is registered for type '" << it->second.assetType.name() 
                << "' but you requested '" << typeid(AssetType).name() << "'";
            return nullptr;
        }

        return dynamic_cast<AssetLoader<AssetType>*>(it->second.loader.get());
    }

    // Load asset directly
    template<typename AssetType>
    std::optional<AssetType> Load(const pe::File* file){
        auto* loader = GetLoader<AssetType>(file->extension);
        if(!loader){
            return std::nullopt;
        }
        return loader->LoadAsset(file);
    }

private:
    std::unordered_map<std::string, LoaderEntry> m_Loaders;
};
}
