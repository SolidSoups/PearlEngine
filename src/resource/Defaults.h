#pragma once

#include <memory>

#include "ShaderData.h"
#include "TextureData.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "ServiceLocator.h"
#include "Logger.h"

class Defaults {
private:
  static std::shared_ptr<ShaderData> m_DefaultShader;
  static std::shared_ptr<TextureData> m_WhiteTexture;
  static std::shared_ptr<TextureData> m_BlackTexture;
  static std::shared_ptr<TextureData> m_NormalTexture;

public:
  static void Init() {
    m_DefaultShader = ServiceLocator::Get<ShaderManager>().load(
        "shaders/vert.glsl", "shaders/frag.glsl");

    m_WhiteTexture = std::make_shared<TextureData>();
    m_BlackTexture = std::make_shared<TextureData>();
    m_NormalTexture = std::make_shared<TextureData>();

    // disable mipmaps in config (1x1 like bruh)
    TextureConfig config;
    config.generateMipMaps = false;
    m_WhiteTexture->setConfig(config);
    m_BlackTexture->setConfig(config);
    m_NormalTexture->setConfig(config);

    // load data
    unsigned char whitePixel[] = {255, 255, 255, 255};
    unsigned char blackPixel[] = {0, 0, 0, 255};
    // --in tangent space this converts to (0, 0, 1) which is straight up
    unsigned char normalPixel[] = {128, 128, 255, 255};
    m_WhiteTexture->loadData(whitePixel, 1, 1, 4);
    m_BlackTexture->loadData(blackPixel, 1, 1, 4);
    m_NormalTexture->loadData(normalPixel, 1, 1, 4);
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
  static std::shared_ptr<TextureData> getNormalTexture() {
    return m_NormalTexture;
  }
};
