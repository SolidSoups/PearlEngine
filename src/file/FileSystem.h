#pragma once

// static class
#include <cstddef>
#include <vector>
#include <string>

class FileSystem {

public:
  static bool loadFile(const char* filePath, std::vector<char>& outBytes);
  static bool writeFile(const char* filePath, const std::vector<char> &bytes);

public:
  struct FileDescriptor {
    std::string stem;
    std::string extension;
    std::string localPath; // eg. asset/miedeval house
    FileDescriptor(std::string _stem, std::string _extension, std::string _path)
      : stem(_stem), extension(_extension), localPath(_path){}
    FileDescriptor() = default;
    std::string getFullName() const { return stem + extension; }
  };
  static std::vector<FileDescriptor> getAllFiles();
  static std::vector<FileDescriptor> queryFiles(const char* extension);

};
