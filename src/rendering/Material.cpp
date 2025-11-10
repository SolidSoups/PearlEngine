#include "Material.h"
#include "ResourceSystem.h"

#include <iostream>

Material::Material(ShaderHandle shaderHandle)
  : m_ShaderHandle(shaderHandle)
{}

// uniform setters
void Material::SetFloat(const std::string& name, float value)           { m_Floats[name] = value; }
void Material::SetVec3(const std::string& name, const glm::vec3& value) { m_Vec3s[name] = value; }
void Material::SetVec4(const std::string& name, const glm::vec4& value) { m_Vec4s[name] = value; }
void Material::SetInt(const std::string& name, int value)               { m_Ints[name] = value; }
void Material::SetTexture(const std::string& name, const TextureHandle& value)        { m_Textures[name] = value; }

void Material::Bind(){
  ShaderData* shaderData = ResourceSystem::Get().Shaders().Get(m_ShaderHandle);
  if(!shaderData){
    std::cerr << "Material::Bind() -> Shader data is invalid" << "\n";
    return;
  }

  UseShader(*shaderData);

  // Upload all ints
  for(const auto& [name, value] : m_Ints){
    ShaderSetInt(*shaderData, name.c_str(), value);
  }

  // Upload all floats
  for(const auto& [name, value] : m_Floats){
    ShaderSetFloat(*shaderData, name.c_str(), value);
  }

  // Upload all vec3s
  for(const auto& [name, value] : m_Vec3s){
    ShaderSetVec3(*shaderData, name.c_str(), value);
  }

  // Upload all vec4s
  for(const auto& [name, value] : m_Vec4s){
    ShaderSetVec4(*shaderData, name.c_str(), value);
  }

  // Upload all textures
  int textureSlot = 0;
  for(const auto& [name, textureHandle] : m_Textures){
    TextureData* data = ResourceSystem::Get().Textures().Get(textureHandle);
    if(!data)
      continue;

    BindTexture(*data, textureSlot);
    ShaderSetInt(*shaderData, name.c_str(), textureSlot);
    textureSlot++;
  }
}

void Material::Unbind(){
  // unbind all textures
  for(int i = 0; i < m_Textures.size(); i++){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  glUseProgram(0);
}
