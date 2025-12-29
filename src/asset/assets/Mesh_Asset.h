#pragma once

#include "IAsset.h"
#include "ISerializationReader.h"

#include <vector>
#include <utility>

struct Mesh_Asset : public IAsset{
    ASSET_CLASS(Mesh_Asset)
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Mesh_Asset(std::vector<float> _vertices, std::vector<unsigned int> _indices) 
        : vertices(std::move(_vertices)), indices(std::move(_indices)) {}
    Mesh_Asset() = default;

public:
    void AcceptSerializer(ISerializationWriter* visitor) override{
        visitor->WriteFloats("vertices", vertices);
        visitor->WriteInt32s("indices", indices);
    }
    void AcceptDeserializer(ISerializationReader* visitor) override{
        visitor->ReadFloats("vertices", vertices);
        visitor->ReadInt32s("indices", indices);
    }
};
