#include "TextureCreator.h"

#include <glad/glad.h>
#include <imgui.h>

#include "UserGUI.h"
#include "TextureManager.h"
#include "ServiceLocator.h"


GLenum convertFilterMode(int index) {
  const GLenum filters[] = {
    GL_NEAREST, GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
  };
  return filters[index];
}

GLenum convertWrapMode(int index){
  const GLenum wraps[] = {
    GL_REPEAT, GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
  };
  return wraps[index];
}

void TextureCreator::renderImGui(const char* id){
  ImGui::PushID(id);
  ImGui::SeparatorText("Source File");
  if(UserGUI::DrawFile(filePath))
			b_isDirty = true;

  ImGui::Dummy(ImVec2(0, 20.f));
  ImGui::SeparatorText("Mipmap settings");

  if(ImGui::Checkbox("Enable Mipmaps", &config.generateMipMaps))
			b_isDirty = true;


  if(config.generateMipMaps){
    const char* filters[] = {
      "Nearest",
      "Linear",
      "Nearest Mipmap Nearest",
      "Nearest Mipmap Linear",
      "Linear Mipmap Nearest",
      "Linear Mipmap Linear"
    };

    static int minFilterIndex = 1;
    static int magFilterIndex = 1;
    static int maxMipLevel = 1;
    if(ImGui::SliderInt("Max Mip Level", &maxMipLevel, 1, 10)       ||
       ImGui::Combo("Min Filter", &minFilterIndex, filters, 6)      ||
       ImGui::Combo("Mag Filter", &magFilterIndex, filters, 2)      || 
       ImGui::SliderFloat("LOD bias", &config.lodBias, -2.0f, 2.0f) || 
       ImGui::SliderFloat("Min LOD", &config.minLod, -10.0f, 10.0f) ||
       ImGui::SliderFloat("Max LOD", &config.maxLod, 0.0f, 1000.0f) || 
       ImGui::SliderInt("Antisotropic Level", &config.antisotropicLevel, 1, 16))
			b_isDirty = true;
    config.minFilter = convertFilterMode(minFilterIndex);
    config.magFilter = convertFilterMode(magFilterIndex);
    config.maxMipMapLevel = maxMipLevel;
  }

  ImGui::Dummy(ImVec2(0, 20));
  ImGui::SeparatorText("Wrap Modes");

  const char* wrapModes[] = {
    "Repeat",
    "Mirrored Repeat",
    "Clamp to Edge",
    "Clamp to Border"
  };
  static int wrapSIndex = 0;
  static int wrapTIndex = 0;

  if(ImGui::Combo("Wrap S", &wrapSIndex, wrapModes, 4))
			b_isDirty = true;
  if(ImGui::Combo("Wrap T", &wrapTIndex, wrapModes, 4))
			b_isDirty = true;
  config.wrapS = convertWrapMode(wrapSIndex);
  config.wrapT = convertWrapMode(wrapTIndex);
  ImGui::PopID();
}

std::shared_ptr<TextureData> TextureCreator::create(){
  b_isDirty = false;
  // TODO: make this better
  if(filePath.empty()) // just a precaution
    return nullptr;
  return ServiceLocator::Get<TextureManager>().load(filePath.c_str(), config);
}
