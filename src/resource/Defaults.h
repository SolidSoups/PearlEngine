#pragma once

#include <memory>

#include "ShaderData.h"
#include "TextureData.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "ServiceLocator.h"

class Defaults {
private:
  static std::shared_ptr<ShaderData> m_DefaultShader;
  static std::shared_ptr<TextureData> m_WhiteTexture;
  static std::shared_ptr<TextureData> m_BlackTexture;

public:
  static void Init() {
    m_DefaultShader = ServiceLocator::Get<ShaderManager>().load(
        "shaders/vert.glsl", "shaders/frag.glsl");

    // create default white texture
    unsigned char whitePixel[] = {255, 255, 255, 255};
    m_WhiteTexture = std::make_shared<TextureData>(whitePixel, 1, 1, 4, false);

    // create default black texture
    unsigned char blackPixel[] = {0, 0, 0, 255};
    m_BlackTexture = std::make_shared<TextureData>(blackPixel, 1, 1, 4, false);
  }
  static std::shared_ptr<ShaderData> getDefaultShader() {
    return m_DefaultShader;
  }
  static std::shared_ptr<TextureData> getWhiteTexture() {
    return m_WhiteTexture;
  }
  static std::shared_ptr<TextureData> getBlackTexture() {
    return m_BlackTexture;
  }
};
