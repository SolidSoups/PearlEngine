#pragma once

#include <memory>

#include "ShaderData.h"
#include "TextureData.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "ServiceLocator.h"

class DefaultResources {
private:
  std::shared_ptr<ShaderData> m_DefaultShader;
  std::shared_ptr<TextureData> m_WhiteTexture;
  std::shared_ptr<TextureData> m_BlackTexture;

public:
  DefaultResources() {
    m_DefaultShader = ServiceLocator::Get<ShaderManager>().load(
        "shaders/vert.glsl", "shaders/frag.glsl");

    // create default white texture
    unsigned char whitePixel[] = {255, 255, 255, 255};
    m_WhiteTexture = std::make_shared<TextureData>(whitePixel, 1, 1, 4, false);

    // create default black texture
    unsigned char blackPixel[] = {0, 0, 0, 255};
    m_BlackTexture = std::make_shared<TextureData>(blackPixel, 1, 1, 4, false);
  }
  std::shared_ptr<ShaderData> getDefaultShader() const {
    return m_DefaultShader;
  }
  std::shared_ptr<TextureData> getWhiteTexture() const { return m_WhiteTexture; }
  std::shared_ptr<TextureData> getBlackTexture() const { return m_BlackTexture; }

};
