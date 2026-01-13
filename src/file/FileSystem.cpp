#include "FileSystem.h"
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <cstring>

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
std::vector<FileSystem::FileDescriptor> FileSystem::getAllFiles(){
  const std::filesystem::path root{"assets"};

  std::error_code ec;
  auto iterator = std::filesystem::recursive_directory_iterator{root, ec};

  if(ec){
    LOG_ERROR 
      << "Failed to open directory '{" 
      << root.string() << "}': " << ec.message().c_str();
    return {};
  }

  std::vector<FileDescriptor> files;
  for(auto const& dir_entry : iterator){
    const auto& path = dir_entry.path();
    files.emplace_back(path.stem(), path.extension(), path.relative_path());
  }
  return files;
}
std::vector<FileSystem::FileDescriptor> FileSystem::queryFiles(const std::string& extension){
  const std::filesystem::path root{"assets"};

  std::error_code ec;
  auto iterator = std::filesystem::recursive_directory_iterator{root, ec};

  if(ec){
    LOG_ERROR 
      << "Failed to open directory '{" 
      << root.string() << "}': " << ec.message().c_str();
    return {};
  }

  std::vector<FileDescriptor> files;
  for(auto const& dir_entry : iterator){
    const auto& path = dir_entry.path();
    std::string ext = path.extension().string();

    if(ext != extension) continue;

    files.emplace_back(path.stem(), path.extension(), path.relative_path());
  }
  return files;
}
std::vector<FileSystem::FileDescriptor> FileSystem::queryFiles(const std::vector<std::string>& extensions){
  const std::filesystem::path root{"assets"};

  std::error_code ec;
  auto iterator = std::filesystem::recursive_directory_iterator{root, ec};

  if(ec){
    LOG_ERROR 
      << "Failed to open directory '{" 
      << root.string() << "}': " << ec.message().c_str();
    return {};
  }

  std::vector<FileDescriptor> files;
  for(auto const& dir_entry : iterator){
    const auto& path = dir_entry.path();
    std::string ext = path.extension().string();

    auto it = std::find(extensions.begin(), extensions.end(), ext);
    if(it == extensions.end()) continue;

    files.emplace_back(path.stem(), path.extension(), path.relative_path());
  }
  return files;
}
