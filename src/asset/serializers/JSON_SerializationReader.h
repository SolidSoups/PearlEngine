#pragma once

#include "ISerializationReader.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "Logger.h"

class JSON_SerializationReader : public ISerializationReader {
  private:
    rapidjson::Document m_Document;

  public:
    JSON_SerializationReader(const std::vector<uint8_t> &bytes) {
        std::string jsonString(bytes.begin(), bytes.end());
        m_Document.Parse(jsonString.c_str());

        if (m_Document.HasParseError()) {
            LOG_ERROR << "JSON Parse Error" << m_Document.GetParseError();
        }
    }

  public:
    void ReadString(const std::string &name, std::string &value) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsString()) {
            value = m_Document[name.c_str()].GetString();
        }
    };
    void ReadInt32(const std::string &name, uint32_t &value) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsUint()) {
            value = m_Document[name.c_str()].GetUint();
        }
    };
    void ReadInt64(const std::string &name, uint64_t &value) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsUint64()) {
            value = m_Document[name.c_str()].GetUint64();
        }
    };
    void ReadSignedInt32(const std::string &name, int32_t &value) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsInt()) {
            value = m_Document[name.c_str()].GetInt();
        }
    };
    void ReadSignedInt64(const std::string &name, int64_t &value) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsInt64()) {
            value = m_Document[name.c_str()].GetInt64();
        }
    };
    void ReadFloat(const std::string &name, float &value) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsFloat()) {
            value = m_Document[name.c_str()].GetFloat();
        }
    };
    void ReadDouble(const std::string &name, double &value) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsDouble()) {
            value = m_Document[name.c_str()].GetDouble();
        }
    };
    void ReadBool(const std::string &name, bool &value) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsBool()) {
            value = m_Document[name.c_str()].GetBool();
        }
    };
    void ReadData(const std::string &name, uint8_t *data,
                  size_t &size) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsString()) {
            std::string hexStr = m_Document[name.c_str()].GetString();
            size_t dataSize = hexStr.length() / 2;

            for (size_t i = 0; i < dataSize; i++) {
                std::string byteStr = hexStr.substr(i * 2, 2);
                data[i] = (uint8_t)std::stoi(byteStr, nullptr, 16);
            }
            size = dataSize;
        }
    };

    // several values
    void ReadFloats(const std::string &name,
                    std::vector<float> &values) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsArray()) {
            values.clear();
            auto array = m_Document[name.c_str()].GetArray();

            for (auto &v : array) {
                if (v.IsFloat() || v.IsDouble()) {
                    values.push_back(v.GetFloat());
                }
            }
        }
    };
    void ReadInt32s(const std::string &name,
                    std::vector<uint32_t> &values) override {
        if (m_Document.HasMember(name.c_str()) &&
            m_Document[name.c_str()].IsArray()) {
            values.clear();

            for(auto& v : m_Document[name.c_str()].GetArray()){
                if(v.IsUint()){
                    values.push_back(v.GetUint());
                }
            }
        }
    };
};
