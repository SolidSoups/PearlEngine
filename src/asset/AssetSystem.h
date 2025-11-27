#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ostream>

#include "File.h"
#include "IAssetConverter.h"
#include "JSON_AssetSerializer.h"
#include "Logger.h"

namespace pe {

using AssetConverter = IAssetConverter;
using Extension = std::string;

class AssetSystem {
private:
    const std::string c_AssetsRoot = "project_assets/";
  public:
    // will import the file and save it to the assets directory
    void ImportAsset(const FileDescriptor *file) {
        // do we have a converter for this file extension?
        AssetConverter *converter;
        if (auto search = m_AssetConverters.find(file->extension);
            search != m_AssetConverters.end()) {
            converter = search->second.get();
        } else {
            // throw an error
            LOG_WARNING << "No asset converter for file extension '" << file->extension << "' was found.";
            return;
        }
        // convert asset
        auto asset = converter->ConvertToAsset(file);

        // serialize asset (JSON for now)
        JSON_AssetSerializer serializer;
        std::vector<uint8_t> bytes = serializer.Serialize(asset.get());

        std::string dest = c_AssetsRoot + file->GetFullName() + ".json";
        try{
            std::ofstream outFile(dest.c_str(), std::ios::binary); 
            outFile.exceptions(std::ios::failbit | std::ios::badbit);
            outFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
            outFile.close();
        }
        catch (const std::ios_base::failure& e){
            LOG_ERROR << "Failed to write asset file: " << e.what();
            return;
        }
        catch (const std::exception& e){
            LOG_ERROR << "Unexpected error writing asset: " << e.what();
            return;
        }

        LOG_INFO << "Succesfully wrote asset file: " << dest;
    }

  public:
    static AssetSystem &Get() {
        static AssetSystem instance;
        return instance;
    }

    // Register any loader of type IAssetLoader
    void RegisterConverter(const std::string &extension,
                           std::unique_ptr<AssetConverter> assetConverter) {
        m_AssetConverters.insert_or_assign(extension,
                                           std::move(assetConverter));
        LOG_INFO << "Registered asset converter for file extension '"
                 << extension << "'";
    }

    // Get the loader and cast to a specific type
    AssetConverter *GetConverter(const std::string &extension) {
        auto it = m_AssetConverters.find(extension);
        if (it == m_AssetConverters.end()) {
            LOG_ERROR << "No asset converter exists for file extension "
                      << extension;
            return nullptr;
        }
        return it->second.get();
    }

  private:
    std::unordered_map<Extension, std::unique_ptr<AssetConverter>>
        m_AssetConverters;
};
} // namespace pe

#define STATIC_INITIALIZE_REGISTER_ASSET_CONVERTER(ExtensionStr,               \
                                                   AssetConverterType)         \
    namespace {                                                                \
    struct AssetConverterType##Registrar {                                     \
        AssetConverterType##Registrar() {                                      \
            pe::AssetSystem::Get().RegisterConverter(                          \
                ExtensionStr, std::make_unique<AssetConverterType>());         \
        }                                                                      \
    };                                                                         \
    static AssetConverterType##Registrar s_##AssetConverterType##Registrar;    \
    }
