#include "Material.h"
#include "Shader.h"

Material::Material(Shader* shader)
  : m_Shader(shader)
{}

// uniform setters
void Material::SetFloat(const std::string& name, float value)           { m_Floats[name] = value; }
void Material::SetVec3(const std::string& name, const glm::vec3& value) { m_Vec3s[name] = value; }
void Material::SetVec4(const std::string& name, const glm::vec4& value) { m_Vec4s[name] = value; }
void Material::SetInt(const std::string& name, int value)               { m_Ints[name] = value; }
void Material::SetTexture(const std::string& name, GLuint value)        { m_Textures[name] = value; }

void Material::Bind(){
  m_Shader->Use();

  // Upload all ints
  for(const auto& [name, value] : m_Ints){
    m_Shader->SetInt(name, value);
  }

  // Upload all floats
  for(const auto& [name, value] : m_Floats){
    m_Shader->SetFloat(name, value);
  }

  // Upload all vec3s
  for(const auto& [name, value] : m_Vec3s){
    m_Shader->SetVec3(name, value);
  }

  // Upload all vec4s
  for(const auto& [name, value] : m_Vec4s){
    m_Shader->SetVec4(name, value);
  }

  // Upload all textures
  int textureSlot = 0;
  for(const auto& [name, textureID] : m_Textures){
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, textureID);
    m_Shader->SetInt(name, textureSlot);
    textureSlot++;
  }
}

void Material::Unbind(){
  glUseProgram(0);

glUseProgram(0);}
