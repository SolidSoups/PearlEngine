#pragma once

#include <iomanip>
#include <ostream>
#include <random>
#include <sstream>
#include <functional>

class UUID {
  private:
    std::string id;

    explicit UUID(const std::string &str) : id(str) {}

  public:
    UUID() : id(GenerateUUID()) {}

    static UUID Null() { return UUID("00000000-0000-0000-0000-000000000000"); }
    static UUID FromString(const std::string &str) { 
        if(str.length() != 36) return Null();
        if(str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-') return Null();
        return UUID(str); 
    }

    const std::string &str() const { return id; }
    bool IsValid() const {
        return id != "00000000-0000-0000-0000-000000000000";
    }

    bool operator==(const UUID &other) const { return id == other.id; }
    bool operator!=(const UUID &other) const { return id != other.id; }
    bool operator<(const UUID &other) const { return id < other.id; }
    friend std::ostream& operator<<(std::ostream& os, const UUID& uuid){
        return os << uuid.id;
    }

  private:
    std::string GenerateUUID() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        std::stringstream ss;
        ss << std::hex;

        // 32 Hex characters with hyphes: 8-4-4-4-12
        for (int i = 0; i < 16; i++) {
            int num = dis(gen);
            if (i == 6)
                num = (num & 0x0F) | 0x40; // Version 4
            if (i == 8)
                num = (num & 0x3F) | 0x80; // Variant bits

            ss << std::setw(2) << std::setfill('0') << num;

            if (i == 3 || i == 5 || i == 7 || i == 9)
                ss << "-";
        }

        return ss.str();
    }
};

namespace std {
template <> struct hash<UUID> {
    size_t operator()(const UUID &uuid) const {
        return std::hash<std::string>()(uuid.str());
    }
};
} // namespace std
