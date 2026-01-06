#include "MaterialData.h"

#include "ResourceSystem.h"
#include <iostream>

#include "Logger.h"

namespace {
MaterialData *GetMaterialData(ResourceSystem* rs, MaterialHandle handle, const char *functionName) {
    MaterialData *data = rs->Get(handle);
    if (!data) {
        std::cerr << "MaterialData.cpp: " << functionName
                  << ": MaterialData is null" << "\n";
    }
    return data;
}
}; // namespace

ShaderHandle MaterialGetShaderHandle(ResourceSystem* rs, MaterialHandle handle) {
    MaterialData *materialData =
        GetMaterialData(rs, handle, "MaterialGetShaderHandle");
    if (!materialData)
        return {};

    return materialData->shaderHandle;
}

void BindMaterial(ResourceSystem* rs, MaterialHandle handle) {
    MaterialData *materialData = GetMaterialData(rs, handle, "BindMaterial");
    if (!materialData)
        return;

    ShaderHandle shaderHandle = materialData->shaderHandle;

    UseShader(rs, shaderHandle);

    // Upload all ints
    for (const auto &[name, value] : materialData->ints) {
        ShaderSetInt(rs, shaderHandle, name.c_str(), value);
    }

    // Upload all floats
    for (const auto &[name, value] : materialData->floats) {
        ShaderSetFloat(rs, shaderHandle, name.c_str(), value);
    }

    // Upload all vec3s
    for (const auto &[name, value] : materialData->vec3s) {
        ShaderSetVec3(rs, shaderHandle, name.c_str(), value);
    }

    // Upload all vec4s
    for (const auto &[name, value] : materialData->vec4s) {
        ShaderSetVec4(rs, shaderHandle, name.c_str(), value);
    }

    // Upload all textures
    int textureSlot = 0;
    for (const auto &[name, textureHandle] : materialData->textureHandles) {
        BindTexture(rs, textureHandle, textureSlot);
        ShaderSetInt(rs, shaderHandle, name.c_str(), textureSlot);
        textureSlot++;
    }

    // Upload all matrixes
    for (const auto &[name, matrix4] : materialData->mat4s) {
        ShaderSetMatrix4(rs, shaderHandle, name.c_str(), matrix4);
    }
}

void DestroyMaterial(ResourceSystem* rs, MaterialHandle handle) {
    if (!rs->Get(handle))
        return;
    rs->Destroy(handle);
}

void MaterialSetFloat(ResourceSystem* rs, MaterialHandle handle, const std::string &name,
                      float value) {
    MaterialData *materialData = GetMaterialData(rs, handle, "MaterialSetFloat");
    if (!materialData)
        return;
    materialData->floats[name] = value;
}

void MaterialSetInt(ResourceSystem* rs, MaterialHandle handle, const std::string &name, int value) {
    MaterialData *materialData = GetMaterialData(rs, handle, "MaterialSetInt");
    if (!materialData)
        return;
    materialData->ints[name] = value;
}

void MaterialSetVec3(ResourceSystem* rs, MaterialHandle handle, const std::string &name,
                     const glm::vec3 &value) {
    MaterialData *materialData = GetMaterialData(rs, handle, "MaterialSetVec3");
    if (!materialData)
        return;
    materialData->vec3s[name] = value;
}

void MaterialSetVec4(ResourceSystem* rs, MaterialHandle handle, const std::string &name,
                     const glm::vec4 &value) {
    MaterialData *materialData = GetMaterialData(rs, handle, "MaterialSetVec4");
    if (!materialData)
        return;
    materialData->vec4s[name] = value;
}

void MaterialSetTexture(ResourceSystem* rs, MaterialHandle handle, const std::string &name,
                        const TextureHandle &value) {
    MaterialData *materialData = GetMaterialData(rs, handle, "MaterialSetTexture");
    if (!materialData)
        return;
    materialData->textureHandles[name] = value;
}

void MaterialSetMat4(ResourceSystem* rs, MaterialHandle handle, const std::string &name,
                     const glm::mat4 &value) {
    MaterialData *materialData = GetMaterialData(rs, handle, "MaterialSetMat4");
    if (!materialData)
        return;
    materialData->mat4s[name] = value;
}
