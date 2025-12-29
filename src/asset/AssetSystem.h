#pragma once

#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "AssetDescriptor.h"
#include "FileDescriptor.h"
#include "FileIO.h"
#include "IAssetConverter.h"
#include "JSON_SerializationReader.h"
#include "JSON_SerializationWriter.h"
#include "Logger.h"
#include "StaticRegistry.h"

namespace pe {
using AssetsRegister = std::vector<AssetDescriptor>;
using ConverterRegistry = StaticRegistry<std::string, IAssetConverter>;

class AssetSystem {
  public:
    // the registry of asset converters to use
    // every file extension needs a converter
    // to preprocess the file into an applicable format
    ConverterRegistry AssetConverters;

  private:
    // Tracks all assets in the assets directory
    AssetsRegister m_ScannedAssets;
    // The root of the assets directory
    const std::filesystem::path c_AssetsRoot{"project_assets"};

  public:
    // scan assets on initialization
    AssetSystem() { ScanAssets(); }

    // Meyer's singleton getter
    static AssetSystem &Get() {
        static AssetSystem instance;
        return instance;
    }
    // Get the scanned assets
    const AssetsRegister &GetAssetsDescriptors() const {
        return m_ScannedAssets;
    }

    // Finds all assets in the assets root folder and
    void ScanAssets();
    // returns an IAsset from the serialized assets directory
    std::unique_ptr<IAsset> LoadAsset(const AssetDescriptor *assetDesc);
    // import an asset from the filesystem, will do all preprocessing necessary
    void ImportAsset(const FileDescriptor *file);
};
} // namespace pe

// helps staticly initialize registration of asset converters
// create a header file, put this in the bottom and boom, its working
#define STATIC_INITIALIZE_REGISTER_ASSET_CONVERTER(ExtensionStr,               \
                                                   AssetConverterType)         \
    namespace {                                                                \
    struct AssetConverterType##Registrar {                                     \
        AssetConverterType##Registrar() {                                      \
            pe::AssetSystem::Get().AssetConverters.Register(                   \
                ExtensionStr, std::make_unique<AssetConverterType>());         \
        }                                                                      \
    };                                                                         \
    static AssetConverterType##Registrar s_##AssetConverterType##Registrar;    \
    }
