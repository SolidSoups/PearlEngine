#pragma once

#include "Handle.h"
#include "ResourceTraits.h"

#define PEARL_DEFINE_RESOURCE(Name, Datastruct, AssetType)                     \
    struct Name##Tag {};                                                       \
    using Name##Handle = Handle<Name##Tag>;                                    \
    template <> struct ResourceTraits<Datastruct> {                            \
        using Tag = Name##Tag;                                                 \
    };                                                                         \
    template <> struct TagToData<Name##Tag> {                                  \
        using Data = Datastruct;                                               \
    };                                                                         \
    template <> struct DataToAsset<Datastruct> {                               \
        using Asset = AssetType;                                               \
    };                                                                         \
    template <> struct AssetToData<AssetType> {                                \
        using Data = Datastruct;                                               \
    };
