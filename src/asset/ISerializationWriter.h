#pragma once

#include <string>
#include <cstdint>
#include <vector>

class ISerializationWriter{
public:
    virtual ~ISerializationWriter() = default;

public:
    // single values
    virtual void WriteString(const std::string& name, const std::string& value) = 0;
    virtual void WriteInt32(const std::string& name, uint32_t value) = 0;
    virtual void WriteInt64(const std::string& name, uint64_t value) = 0;
    virtual void WriteSignedInt32(const std::string& name, int32_t value) = 0;
    virtual void WriteSignedInt64(const std::string& name, int64_t value) = 0;
    virtual void WriteFloat(const std::string& name, float value) = 0;
    virtual void WriteDouble(const std::string& name, double value) = 0;
    virtual void WriteBool(const std::string& name, bool value) = 0;
    virtual void WriteData(const std::string& name, const uint8_t* data, size_t size) = 0;

    // several values
    virtual void WriteFloats(const std::string& name, const std::vector<float>& values) = 0;
    virtual void WriteInt32s(const std::string& name, const std::vector<uint32_t>& values) = 0;

public:
    virtual std::vector<uint8_t> GetBytes() const = 0;
};
