#include "Project.h"
#include <filesystem>
#include <system_error>

#include "Logger.h"


pe::Project::Project(){
    ScanFilesystem();
}

void pe::Project::ScanFilesystem(){
    m_Files.clear();

    std::error_code ec;
    auto iterator = std::filesystem::recursive_directory_iterator{m_Root, ec};

    if(ec){
        LOG_ERROR << "Failed to open directory '{" << m_Root.string() << "}': {" << ec.message() << "}";
        return;
    }

    for(const auto& dirEntry : iterator){
        const auto& path = dirEntry.path();
        std::string extension{path.extension()};
        if(allowedExtensions.count(extension) == 0){
            continue;
        }
        m_Files.emplace_back(path.stem(), extension, path);
    }
}

const std::vector<const pe::FileDescriptor*> pe::Project::FindFileByStem(const std::string& stem) const{
    std::vector<const pe::FileDescriptor*> results;
    for(const auto& file : m_Files){
        if(file.stem == stem){
            results.push_back(&file);
        }
    }
    return results;
}
const std::vector<const pe::FileDescriptor*> pe::Project::FindFileByExtension(const std::string& extension) const{
    std::vector<const pe::FileDescriptor*> results;
    for(const auto& file : m_Files){
        if(file.extension == extension){
            results.push_back(&file);
        }
    }
    return results;
}
const pe::FileDescriptor* pe::Project::FindFile(const std::string& stem, const std::string& extension) const{
    for(const auto& file : m_Files){
        if(file.stem == stem && file.extension == extension){
            return &file;
        }
    }
    return nullptr;
}
const pe::FileDescriptor* pe::Project::FindFileByPath(const std::filesystem::path& relativePath) const{
    for(const auto& file : m_Files){
        if(file.localPath == m_Root / relativePath){
            return &file;
        }
    }
    return nullptr;
}
