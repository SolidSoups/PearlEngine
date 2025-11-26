#pragma once

#include <string>
#include <filesystem>

#include "UUID.h"

namespace pe{
using FileHandle = UUID;
struct FileDescriptor{
    std::string stem;
    std::string extension;
    std::filesystem::path localPath;
    FileHandle uuid;

    FileDescriptor(const std::string& nm, const std::string& ext, std::filesystem::path path) 
    : stem(nm), extension(ext), localPath(path){}

    std::string GetFullName() const { return stem + extension; }
};
}
