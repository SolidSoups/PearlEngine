#pragma once

#include "ResourceTraits.h"

#define PEARL_DEFINE_RESOURCE(Name, Datastruct) \
    struct Name##Tag {}; \
    using Name##Handle = Handle<Name##Tag>; \
    template <> \
    struct ResourceTraits<Datastruct> { \
        using Tag = Name##Tag; \
    }; \
    template <> \
    struct TagToData<Name##Tag> { \
        using Data = Datastruct; \
    };

