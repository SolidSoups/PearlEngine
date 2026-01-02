#pragma once

#include <fstream>
#include <stdint.h>

#include "Logger.h"

class FileIO{

public:
    // Private helper function for writing out an asset file
    // Returns true if succesful.
    static bool WriteFile(const char* destinationPath, const std::vector<uint8_t> &bytes){
        // pretty much anything can happen when writing files
        // try-catch will add some railings
        try{
            std::ofstream outFile(destinationPath, std::ios::binary);
            // enable exceptions
            outFile.exceptions(std::ios::failbit | std::ios::badbit);
            outFile.write(reinterpret_cast<const char *>(bytes.data()),
                          bytes.size());
            // close the file, it will happen every time since std::ofstream is scoped
            outFile.close();
        } catch (const std::ios_base::failure &e) {
            LOG_ERROR << "Failed to write file: " << e.what();
            return false;
        } catch (const std::exception &e) {
            LOG_ERROR << "Unexpected error writing file: " << e.what();
            return false;
        }

        return true;
    }

    static std::vector<uint8_t> ReadFile(const char* path){
        std::ifstream inFile{path, std::ios::binary};
        inFile.exceptions(std::ios::failbit | std::ios::badbit);

        // Get the file size and read all bytes
        inFile.seekg(0, std::ios::end);
        size_t fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        std::vector<uint8_t> bytes(fileSize);
        inFile.read(reinterpret_cast<char*>(bytes.data()), fileSize);
        return bytes;
    }

};
