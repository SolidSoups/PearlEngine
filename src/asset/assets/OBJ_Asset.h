#pragma once

#include "IAsset.h"

#include <vector>

struct OBJ_Asset : public IAsset{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

public:
    void AcceptSerializer(ISerializationWriter* visitor){
        // noop
        visitor->WriteFloats("vertices", vertices);
        visitor->WriteInt32s("indices", indices);
    }
};
