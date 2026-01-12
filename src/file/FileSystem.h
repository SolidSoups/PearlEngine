#pragma once

// static class
#include <cstddef>
#include <vector>
class FileSystem {

public:
  static bool loadFile(const char* filePath, std::vector<char>& outBytes);
  static bool writeFile(const char* filePath, const std::vector<char> &bytes);

public:
  struct FileDescriptor {
    const char* stem;
    const char* extension;
    const char* localPath; // eg. asset/miedeval house
    FileDescriptor(const char* _stem, const char* _extension, const char* _path)
      : stem(_stem), extension(_extension), localPath(_path){}
  };
  static std::vector<FileDescriptor> getAllFiles();
  static std::vector<FileDescriptor> queryFiles(const char* extension);

};
