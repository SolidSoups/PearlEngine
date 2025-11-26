
// src
#include "JSON_AssetSerializer.h"
#include "PearlEngine.h"
#include "TEST_Asset.h"

#include <cstdint>
#include <iostream>

int main() {
    PearlEngine engine;
    if (!engine.IsInitialized()) {
        return -1;
    }

    TEST_Asset testAsset{12, "hello", 3.1415f};
    JSON_AssetSerializer assetSerializer;
    std::vector<uint8_t> serializedOutput = assetSerializer.Serialize(&testAsset);
    std::cout << "\e[0;31m";
    for(const auto& val : serializedOutput){
        std::cout << val;
    }
    std::cout << "\e[0;37m" << std::endl;

    // engine.RunUpdateLoop();
    return 0;
}
