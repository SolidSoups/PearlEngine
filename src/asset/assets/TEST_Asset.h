#pragma once

#include "IAsset.h"

struct TEST_Asset : public IAsset{
    int myInt;
    std::string myString;
    float myFloat;
    std::vector<float> myFloats;

    TEST_Asset(int i, std::string s, float f) : myInt(i), myString(s), myFloat(f){}
    void SetInts(std::vector<float> ints){
        myFloats = ints;
    }

public:
    void AcceptSerializer(ISerializationWriter* visitor){
        visitor->WriteInt32("myInt", myInt);
        visitor->WriteString("myString", myString);
        visitor->WriteFloat("myFloat", myFloat);
        visitor->WriteFloats("myFloats", myFloats);
    };
};
