#pragma once

#include "ISerializationWriter.h"
#include "rapidjson/rapidjson.h"

#include <iomanip>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

class JSON_SerializationWriter : public ISerializationWriter {
  private:
    rapidjson::Document m_Document;
    rapidjson::Value *currentObject_;

  public:
    JSON_SerializationWriter() {
        m_Document.SetObject();
        currentObject_ = &m_Document;
    }

  private:
    template <typename T> void GenericWrite(const std::string &name, T value) {
        rapidjson::Value key(name.c_str(), m_Document.GetAllocator());
        currentObject_->AddMember(key, value, m_Document.GetAllocator());
    }
    template <typename T> void GenericWriteArray(const std::string &name, const std::vector<T>& values){
        rapidjson::Value array(rapidjson::kArrayType);
        for(const T& val : values){
            array.PushBack(val, m_Document.GetAllocator());
        }
        rapidjson::Value key(name.c_str(), m_Document.GetAllocator());
        currentObject_->AddMember(key, array, m_Document.GetAllocator());
    }

  public:
    void WriteString(const std::string &name,
                     const std::string &value) override {
        rapidjson::Value key(name.c_str(), m_Document.GetAllocator());
        rapidjson::Value val(value.c_str(), m_Document.GetAllocator());
        currentObject_->AddMember(key, val, m_Document.GetAllocator());
    }
    void WriteInt32(const std::string &name, uint32_t value) override {
        GenericWrite(name, value);
    }
    void WriteInt64(const std::string &name, uint64_t value) override {
        GenericWrite(name, value);
    }
    void WriteSignedInt32(const std::string &name, int32_t value) override {
        GenericWrite(name, value);
    }
    void WriteSignedInt64(const std::string &name, int64_t value) override {
        GenericWrite(name, value);
    }
    void WriteFloat(const std::string &name, float value) override {
        GenericWrite(name, value);
    }
    void WriteDouble(const std::string &name, double value) override {
        GenericWrite(name, value);
    }
    void WriteBool(const std::string &name, bool value) override {
        GenericWrite(name, value);
    }
    void WriteData(const std::string &name, const unsigned char *data,
                   size_t size) override {
        std::stringstream ss;
        for (size_t i = 0; i < size; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
        }
        rapidjson::Value key(name.c_str(), m_Document.GetAllocator());
        rapidjson::Value value(ss.str().c_str(), m_Document.GetAllocator());
        currentObject_->AddMember(key, value, m_Document.GetAllocator());
    }


    void WriteFloats(const std::string& name, const std::vector<float>& values) override{
        GenericWriteArray(name, values);
    }
    void WriteInt32s(const std::string& name, const std::vector<uint32_t>& values) override{
        GenericWriteArray(name, values);
    }

  public:
    std::vector<uint8_t> GetBytes() const override {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        m_Document.Accept(writer);

        std::string json = buffer.GetString();
        return std::vector<uint8_t>(json.begin(), json.end());
    }
};
