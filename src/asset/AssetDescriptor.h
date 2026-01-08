#pragma once

#include <filesystem>

#include "UUID.h"

namespace pe {
using AssetHandle = UUID;
struct AssetDescriptor {
  std::string stem;
  std::string type;
  std::string extension;
  std::filesystem::path localPath;

  AssetDescriptor(const std::string &stem, const std::string& type,const std::string &extension,
                  const std::filesystem::path &localPath)
      : stem(stem), type(type), extension(extension),
        localPath(localPath) {}
};
} // namespace pe
