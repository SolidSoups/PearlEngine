#pragma once

#include "IAssetSerializer.h"
#include "JSON_SerializationWriter.h"

class JSON_AssetSerializer : public IAssetSerializer{
public:
    const std::vector<uint8_t> Serialize(IAsset* asset)override {
        std::unique_ptr<JSON_SerializationWriter> writer = std::make_unique<JSON_SerializationWriter>();
        writer->WriteString("header", "SerializedObject");
        asset->AcceptSerializer(writer.get());
        std::string json = writer->GetJSONString();

        return std::vector<uint8_t>(json.begin(), json.end());
    }
    std::unique_ptr<IAsset> Deserialize(const std::vector<uint8_t>& data)override {
        // noop for now
        return nullptr;
    }
};
