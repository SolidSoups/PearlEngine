#pragma once

#include "Assets.h"

#define STATIC_REGISTER_ASSET_LOADER(Extension, AssetLoaderType, AssetType) \
namespace { \
    struct AssetLoaderType##Registrar{ \
        AssetLoaderType##Registrar() { \
            pe::Assets::Get().RegisterLoader<AssetType>(Extension, std::make_unique<AssetLoaderType>()); \
        }; \
    }; \
    static AssetLoaderType##Registrar s_##AssetLoaderType##Registrar; \
}
