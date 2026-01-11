#pragma once

// static class
#include <cstddef>
#include <vector>
class FileSystem {

public:
  static bool loadFile(const char* filePath, std::vector<char>& outBytes);
  static bool writeFile(const char* filePath, const std::vector<char> &bytes);
};
