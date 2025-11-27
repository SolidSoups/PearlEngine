#pragma once

#include "IAsset.h"

#include <vector>
#include <utility>

struct OBJ_Asset : public IAsset{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    OBJ_Asset(std::vector<float> _vertices, std::vector<unsigned int> _indices) 
        : vertices(std::move(_vertices)), indices(std::move(_indices)) {}

public:
    void AcceptSerializer(ISerializationWriter* visitor){
        // noop
        visitor->WriteFloats("vertices", vertices);
        visitor->WriteInt32s("indices", indices);
    }
};
