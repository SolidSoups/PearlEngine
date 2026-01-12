#include "MaterialData.h"
#include "Logger.h"

MaterialData::MaterialData(std::shared_ptr<ShaderData> _shader) : shader(_shader) {}

void MaterialData::bind() {
    if (!shader) {
        LOG_ERROR << "MaterialData::bind: shader is null";
        return;
    }

    shader->use();

    // Upload all ints
    for (const auto &[name, value] : ints) {
        shader->setInt(name.c_str(), value);
    }

    // Upload all floats
    for (const auto &[name, value] : floats) {
        shader->setFloat(name.c_str(), value);
    }

    // Upload all vec3s
    for (const auto &[name, value] : vec3s) {
        shader->setVec3(name.c_str(), value);
    }

    // Upload all vec4s
    for (const auto &[name, value] : vec4s) {
        shader->setVec4(name.c_str(), value);
    }

    // Upload all textures
    int textureSlot = 0;
    for (const auto &[name, texture] : textures) {
        if (texture) {
            texture->bind(textureSlot);
            shader->setInt(name.c_str(), textureSlot);
            textureSlot++;
        }
    }

    // Upload all matrixes
    for (const auto &[name, matrix4] : mat4s) {
        shader->setMatrix4(name.c_str(), matrix4);
    }
}

void MaterialData::setFloat(const std::string &name, float value) {
    floats[name] = value;
}

void MaterialData::setInt(const std::string &name, int value) {
    ints[name] = value;
}

void MaterialData::setVec3(const std::string &name, const glm::vec3 &value) {
    vec3s[name] = value;
}

void MaterialData::setVec4(const std::string &name, const glm::vec4 &value) {
    vec4s[name] = value;
}

void MaterialData::setTexture(const std::string &name, std::shared_ptr<TextureData> texture) {
    textures[name] = texture;
}

void MaterialData::setMat4(const std::string &name, const glm::mat4 &value) {
    mat4s[name] = value;
}
