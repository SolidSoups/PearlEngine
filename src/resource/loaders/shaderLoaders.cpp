#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "ResourceSystem.h"

#include "Logger.h"

namespace { // empty namespace will only be visible to this translation unit
// Read a shader file
std::string ReadShaderFile(const char *filepath) {
    std::string code;
    std::ifstream shaderFile;
    // ensure ifstreams can throw exceptions
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filepath);
        std::stringstream ss;

        ss << shaderFile.rdbuf();

        shaderFile.close();

        code = ss.str();
    } catch (std::ifstream::failure e) {
        LOG_ERROR << "ShaderData.cpp: ReadShaderFile: File failed to read\n"
                  << e.what();
        return "";
    }

    return code;
};

enum ShaderType { VERTEX, FRAGMENT };
GLuint CompileShader(const char *code, ShaderType type) {
    GLuint shaderID;
    int success;
    char infoLog[512];

    // create shader
    int shaderType = type == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &code, NULL);

    // compile shader
    glCompileShader(shaderID);

    // check for errors
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "ShaderData.cpp: CompileShader: "
                  << (type == VERTEX ? "Vertex" : "Fragment")
                  << " compilation failed\n"
                  << infoLog << std::endl;
        return 0;
    }

    return shaderID;
}

ShaderData *GetShaderData(ResourceSystem* rs, ShaderHandle handle, const char *functionName) {
    ShaderData *data = rs->Get(handle);
    if (!data) {
        std::cerr << "ShaderData.cpp: " << functionName
                  << ": ShaderData is null" << "\n";
    }
    return data;
}
}; // namespace

ShaderHandle CreateShader(ResourceSystem* rs, const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode = ReadShaderFile(vertexPath);
    std::string fragmentCode = ReadShaderFile(fragmentPath);

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // compile shaders
    GLuint vert = CompileShader(vShaderCode, VERTEX);
    GLuint frag = CompileShader(fShaderCode, FRAGMENT);

    // create shader program
    GLuint shaderObjectID = glCreateProgram();

    // attach and link program with compiled shaders
    glAttachShader(shaderObjectID, vert);
    glAttachShader(shaderObjectID, frag);
    glLinkProgram(shaderObjectID);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderObjectID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderObjectID, 512, NULL, infoLog);
        std::cout << "ShaderData.cpp: CreateShader: Program linking failed\n"
                  << infoLog << std::endl;
        return {0};
    }

    // delete the shaders as they're now linked and no longer needed
    glDeleteShader(vert);
    glDeleteShader(frag);

    // create da handle
    ShaderData *newShaderData = new ShaderData(shaderObjectID);
    ShaderHandle shaderHandle =
        rs->Create<ShaderData>(newShaderData);

    LOG_INFO << "Created shader with paths:\n\tvert: " << vertexPath
             << "\n\tfrag: " << fragmentPath;
    return shaderHandle;
}
