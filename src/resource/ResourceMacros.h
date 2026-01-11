#pragma once

#include "Handle.h"

#define PEARL_DEFINE_RESOURCE(DataType) \
  using DataType##Handle = Handle<DataType>;

#define RESOURCE_CLASS(Name) \
  public: \
    Name(const Name&) = delete; \
    Name &operator=(const Name&) = delete;

