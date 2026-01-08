#include "AssetSystem.h"
#include "AssetDescriptor.h"
#include "FileDescriptor.h"
#include "IAsset.h"
#include "JSON_SerializationWriter.h"
#include "Mesh_Asset.h"
#include "Texture_Asset.h"
#include "converters/Texture_AssetConverter.h"
#include <optional>

void pe::AssetSystem::ScanAssets() {
  m_ScannedAssets.clear();

  // get a recursive (goes deep) directory iterator and check for erros
  std::error_code ec;
  auto iterator =
      std::filesystem::recursive_directory_iterator{c_AssetsRoot, ec};

  // check for error (io or permissions issue or other...)
  if (ec) {
    LOG_ERROR << "Failed to open directory '{" << c_AssetsRoot.string()
              << "}': " << ec.message();
    return;
  }

  // iterate over every entry in directory
  for (const auto &dirEntry : iterator) {
    const auto &path = dirEntry.path();

    auto desc = GetParsedAssetDescriptor(path.c_str());
    if(desc == std::nullopt)
      continue;
    m_ScannedAssets.push_back(desc.value());
  }
}

std::unique_ptr<IAsset>
pe::AssetSystem::LoadAsset(const AssetDescriptor *assetDesc) {
  // Create the appropriate asset based on descriptor file extension
  IAssetConverter *converter = AssetConverters.Get(assetDesc->type);
  if (!converter) {
    LOG_ERROR << "Failed to find converter for asset at path "
              << assetDesc->localPath;
    return nullptr;
  }
  auto asset = converter->CreateEmptyAsset();

  // Load the file
  std::vector<uint8_t> bytes = FileIO::ReadFile(assetDesc->localPath.c_str());

  // create the serializer
  ISerializationReader *reader = new JSON_SerializationReader(bytes);

  // peek the header and uuid
  std::string headerStr, uuidStr;
  reader->ReadString("header", headerStr);
  reader->ReadString("uuid", uuidStr);
  asset->uuid = UUID::FromString(uuidStr);

  // populate asset properties
  asset->AcceptDeserializer(reader);

  // at this point, we either return the asset or
  // we need to load it into resource memory, ie
  // we need to allocate it
  return asset;
}

std::unique_ptr<IAsset>
pe::AssetSystem::CreateAsset(const FileDescriptor *file) {
  // do we have a converter for this file extension?
  IAssetConverter *converter = AssetConverters.Get(file->extension);
  if (!converter) {
    LOG_ERROR << "No asset converter could be found for extension '"
              << file->extension << "'";
    return nullptr;
  }
  std::unique_ptr<IAsset> asset = converter->ConvertToAsset(file);
  return asset;
}

void pe::AssetSystem::ImportAsset(const FileDescriptor *file) {
  std::unique_ptr<IAsset> asset = CreateAsset(file);
  if (!asset)
    return;
  SaveAsset(asset, file->stem.c_str());
}

void pe::AssetSystem::SaveAsset(std::unique_ptr<IAsset> &asset,
                                const char *name) {
  if (!asset) {
    LOG_ERROR << "Cannot save! Asset is nullptr!";
    return;
  }
  auto writer = std::make_unique<JSON_SerializationWriter>();
  writer->WriteString("header", "SerializedObject");
  writer->WriteString("uuid", asset->uuid.str());
  asset->AcceptSerializer(writer.get());
  std::vector<uint8_t> bytes = writer->GetBytes();

  std::string dest = c_AssetsRoot.string() + "/" + name + "." +
                     asset->GetAssetTypeName() + ".json";

  if (!FileIO::WriteFile(dest.c_str(), bytes)) {
    LOG_ERROR << "File writing failed for asset file: " << dest;
  } else {
    LOG_INFO << "Succesfully wrote asset file: " << dest;
    ScanAssets();
  }
};

std::optional<pe::AssetDescriptor>
pe::AssetSystem::GetParsedAssetDescriptor(const char *path) {
  std::filesystem::path fullPath{path};
  std::string fileName{fullPath.filename().string()};

  size_t lastDot = fileName.rfind('.');
  size_t secondLastDot = fileName.rfind('.', lastDot - 1);
  if (secondLastDot == std::string::npos) {
    LOG_ERROR << "Incorrect format on file: " << path;
    return std::nullopt;
  }

  std::string stem = fileName.substr(0, secondLastDot);
  std::string type =
      fileName.substr(secondLastDot + 1, lastDot - secondLastDot - 1);
  std::string ext = fileName.substr(lastDot);
  return AssetDescriptor{stem, type, ext, path};
}
