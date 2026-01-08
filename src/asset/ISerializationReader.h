#pragma once

#include <cstdint>
#include <string>
#include <vector>

class ISerializationReader {
  public:
    virtual ~ISerializationReader() = default;

  public:
    // single values
    virtual void ReadString(const std::string &name, std::string& value) = 0;
    virtual void ReadInt32(const std::string &name, uint32_t& value) = 0;
    virtual void ReadInt64(const std::string &name, uint64_t& value) = 0;
    virtual void ReadSignedInt32(const std::string &name, int32_t &value) = 0;
    virtual void ReadSignedInt64(const std::string &name, int64_t &value) = 0;
    virtual void ReadFloat(const std::string &name, float &value) = 0;
    virtual void ReadDouble(const std::string &name, double &value) = 0;
    virtual void ReadBool(const std::string &name, bool &value) = 0;
    virtual void ReadData(const std::string &name, std::vector<unsigned char> &data) = 0;

    // several values
    virtual void ReadFloats(const std::string &name,
                            std::vector<float> &values) = 0;
    virtual void ReadInt32s(const std::string &name,
                            std::vector<uint32_t> &values) = 0;
};
