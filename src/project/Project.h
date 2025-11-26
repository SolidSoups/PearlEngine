#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <set>

#include "File.h"


namespace pe {
// std::set has O(1) search capabilities
const std::set<std::string> allowedExtensions{
    ".obj", ".mat", ".png", ".jpg", ".txt"
};
// sort of an API class, but it will need to watch for changes in the file system
class Project{
private:
    std::vector<FileDescriptor> m_Files;
    std::filesystem::path m_Root{"assets"};

    // private constructor, no need to worry about copy/move operations
    Project(); 
public:
    static Project& Get() {
        static Project project;
        return project;
    }

public:
    void ScanFilesystem();

public:
    /*
    *   Searches the file database and returns a vector of every file matching the stem.
    *   Example: FindFileByStem("sunshine") -> finds the file for "sunshine.png"
    */
    const std::vector<const FileDescriptor*> FindFileByStem(const std::string& stem) const;
    /*
     *  Searches the file database and returns a vector of every file matching the extension.
     *  Example: FindFileByExtension(".jpg") -> { "sunshine.jpg", "pearl.jpg" }
     */
    const std::vector<const FileDescriptor*> FindFileByExtension(const std::string& extension) const;
    /*
    *   Searches the file databse and returns the first file matching the stem and extension.
    *   Example: FindFile("sunshine", ".png") -> finds the file for "sunshine.png"
    */
    const FileDescriptor* FindFile(const std::string& stem, const std::string& extension) const ;
    /*
    *   Searches the file database and returns the first file with the same relative path
    *   Example: FindFileByPath("models/kitty.obj") -> finds the file for "assets/models/kitty.obj"
    */
    const FileDescriptor* FindFileByPath(const std::filesystem::path& relativePath) const;

public:
    const std::vector<FileDescriptor> &GetFiles() const { return m_Files; }
    const std::filesystem::path &GetRoot() const { return m_Root; }
};
}

