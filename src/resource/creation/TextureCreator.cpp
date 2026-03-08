#include "TextureCreator.h"

#include <glad/glad.h>
#include <imgui.h>

#include "UserGUI.h"
#include "TextureManager.h"
#include "ServiceLocator.h"

GLenum convertFilterMode(int index) {
  const GLenum filters[] = {GL_NEAREST,
                            GL_LINEAR,
                            GL_NEAREST_MIPMAP_NEAREST,
                            GL_NEAREST_MIPMAP_LINEAR,
                            GL_LINEAR_MIPMAP_NEAREST,
                            GL_LINEAR_MIPMAP_LINEAR};
  return filters[index];
}

GLenum convertWrapMode(int index) {
  const GLenum wraps[] = {GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE,
                          GL_CLAMP_TO_BORDER};
  return wraps[index];
}

void TextureCreator::renderImGui(const char* id, bool showMipmap , bool showWrapModes) {
  // filthy lazy way to do this
  if (!ServiceLocator::IsReady<TextureManager>())
    return;

  if (!mUnknownTexture)
    mUnknownTexture = ServiceLocator::Get<TextureManager>().load(
        "assets/ui/unknown_texture.jpeg");

  std::string header_name = "Texture_" + std::string(id);
  ImGui::PushID(id);

  // draw either default no texture,
  // or the chosen texture
  intptr_t texImg = mUnknownTexture->id;
  if (latestTexture)
    texImg = latestTexture->id;

  static float boxHeight = 100.f;
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
  ImGui::BeginChild("TextureBox", ImVec2(0, boxHeight), true);
  float startY = ImGui::GetCursorPosY();
  ImVec2 size(100, 100);

  float imageWidth = size.x + ImGui::GetStyle().ItemSpacing.x;
  ImGui::BeginChild(
      "##right", ImVec2(ImGui::GetContentRegionAvail().x - imageWidth, size.y),
      false, ImGuiWindowFlags_NoScrollbar);
  ImGui::SeparatorText(id);
  if (UserGUI::DrawFile(filePath, b_fileChanged))
    b_isDirty = true;
  ImGui::EndChild();

  // Right, image
  ImGui::SameLine();
  ImGui::Image((ImTextureRef)texImg, size, ImVec2(0, 1), ImVec2(1, 0));

  // draw mipmap settings
  if (showMipmap and ImGui::CollapsingHeader("MipMap Settings")) {
    if (ImGui::Checkbox("Enable Mipmaps", &config.generateMipMaps))
      b_isDirty = true;

    if (config.generateMipMaps) {
      const char *filters[] = {"Nearest",
                               "Linear",
                               "Nearest Mipmap Nearest",
                               "Nearest Mipmap Linear",
                               "Linear Mipmap Nearest",
                               "Linear Mipmap Linear"};

      static int minFilterIndex = 1;
      static int magFilterIndex = 1;
      static int maxMipLevel = 1;
      if (ImGui::SliderInt("Max Mip Level", &maxMipLevel, 1, 10) ||
          ImGui::Combo("Min Filter", &minFilterIndex, filters, 6) ||
          ImGui::Combo("Mag Filter", &magFilterIndex, filters, 2) ||
          ImGui::SliderFloat("LOD bias", &config.lodBias, -2.0f, 2.0f) ||
          ImGui::SliderFloat("Min LOD", &config.minLod, -10.0f, 10.0f) ||
          ImGui::SliderFloat("Max LOD", &config.maxLod, 0.0f, 1000.0f) ||
          ImGui::SliderInt("Antisotropic Level", &config.antisotropicLevel, 1,
                           16))
        b_isDirty = true;
      config.minFilter = convertFilterMode(minFilterIndex);
      config.magFilter = convertFilterMode(magFilterIndex);
      config.maxMipMapLevel = maxMipLevel;
    }
  }

  if (showWrapModes and ImGui::CollapsingHeader("Wrap Modes")) {
    const char *wrapModes[] = {"Repeat", "Mirrored Repeat", "Clamp to Edge",
                               "Clamp to Border"};
    static int wrapSIndex = 0;
    static int wrapTIndex = 0;

    if (ImGui::Combo("Wrap S", &wrapSIndex, wrapModes, 4))
      b_isDirty = true;
    if (ImGui::Combo("Wrap T", &wrapTIndex, wrapModes, 4))
      b_isDirty = true;
    config.wrapS = convertWrapMode(wrapSIndex);
    config.wrapT = convertWrapMode(wrapTIndex);
  }

  // calculate new box height (adjustable to content)
  boxHeight =
      ImGui::GetCursorPosY() - startY + ImGui::GetStyle().WindowPadding.y * 1.5;
  ImGui::EndChild();
  ImGui::PopStyleColor();

  ImGui::PopID();
}

std::shared_ptr<TextureData> TextureCreator::create() {
  b_isDirty = false;
  // TODO: make this better
  if (filePath.empty()) // just a precaution
    return nullptr;
  latestTexture =
      ServiceLocator::Get<TextureManager>().load(filePath.c_str(), config);
  return latestTexture;
}
