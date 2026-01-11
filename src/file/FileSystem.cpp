#include "FileSystem.h"
#include <fstream>

#include "Logger.h"

bool FileSystem::loadFile(const char *filePath, std::vector<char>& outBytes){
  try{
    std::ifstream inFile{filePath, std::ios::binary};
    inFile.exceptions(std::ios::failbit | std::ios::badbit);

    // get the file size and read all bytes
    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    inFile.read(reinterpret_cast<char*>(bytes.data()), fileSize);
    outBytes = bytes;
  } catch(const std::exception &e){
    LOG_ERROR << "Unexpected error reading file: " << e.what();
    return false;
  }
  return true;
}
bool FileSystem::writeFile(const char* filePath, const std::vector<char> &bytes){
  try{
    std::ofstream outFile(filePath, std::ios::binary);
    outFile.exceptions(std::ios::failbit | std::ios::badbit);
    outFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    outFile.close();
  } catch(const std::ios_base::failure &e){
    LOG_ERROR << "Failed to write file " << e.what();
    return false;
  } catch(const std::exception &e){
    LOG_ERROR << "Unexpected error writing file: " << e.what();
    return false;
  }
  return true;
}
