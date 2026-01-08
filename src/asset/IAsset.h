#pragma once

#include "ISerializationReader.h"
#include "ISerializationWriter.h"
#include "UUID.h"
#include <typeindex>

struct IAsset {
public:
  virtual ~IAsset() = default;
  UUID uuid;
  virtual std::type_index GetTypeIndex() const = 0;
  virtual const char* GetAssetTypeName() const = 0;

  virtual void AcceptSerializer(ISerializationWriter *visitor) = 0;
  virtual void AcceptDeserializer(ISerializationReader *visitor) = 0;
};

#define ASSET_CLASS(Type)                                                      \
public:                                                                        \
  std::type_index GetTypeIndex() const override {                              \
    return std::type_index(typeid(Type));                                      \
  }                                                                            \
  std::type_index GetStaticTypeIndex() const {                                 \
    return std::type_index(typeid(Type));                                      \
  }                                                                            \
  const char *GetAssetTypeName() const override { return #Type; }
