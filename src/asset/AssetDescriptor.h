#pragma once

#include <filesystem>

#include "UUID.h"


namespace pe {
using AssetHandle = UUID;
struct AssetDescriptor {
    std::string stem;
    std::string extension;
    std::string serializationExtension;
    std::filesystem::path localPath;

    AssetDescriptor(const std::string &stem, const std::string &extension,
                    const std::string &serializedExtension, const std::filesystem::path &localPath)
        : stem(stem), extension(extension),
          serializationExtension(serializedExtension), localPath(localPath) {}
};
} // namespace pe
