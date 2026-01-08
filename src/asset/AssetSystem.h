#pragma once

#include <cstddef>
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
class AssetSystem {
public:
  // the registry of asset converters to use
  // every file extension needs a converter
  // to preprocess the file into an applicable format
  StaticRegistry<std::string, IAssetConverter> AssetConverters;

private:
  // Tracks all assets in the assets directory
  std::vector<AssetDescriptor> m_ScannedAssets;
  // The root of the assets directory
  const std::filesystem::path c_AssetsRoot{"project_assets"};

public:
  // scan assets on initialization
  AssetSystem() { ScanAssets(); }

  // Get the scanned assets
  const std::vector<AssetDescriptor> &GetAssetsDescriptors() const { return m_ScannedAssets; }

  // Finds all assets in the assets root folder and
  void ScanAssets();
  // returns an IAsset from the serialized assets directory
  std::unique_ptr<IAsset> LoadAsset(const AssetDescriptor *assetDesc);
  // import an asset from the filesystem, will do all preprocessing necessary
  void ImportAsset(const FileDescriptor *file);
};
} // namespace pe
