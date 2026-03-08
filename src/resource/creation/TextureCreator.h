#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include <glad/glad.h>

#include "TextureData.h"

class TextureCreator {
  std::shared_ptr<TextureData> latestTexture;
  bool b_isDirty = false;
  bool b_fileChanged = false;
  TextureConfig config;
  std::string filePath;

public:
  void renderImGui(const char* id, bool showMipmap = true, bool showWrapModes=true);
  bool isDirty() const { return b_isDirty; }
  std::shared_ptr<TextureData> create();
  void reset() {
    filePath.clear();
    latestTexture.reset();
    config = TextureConfig{};
    b_isDirty = false;
  }
  void syncFrom(std::shared_ptr<TextureData> texture) {
    if (!texture) { reset(); return; }
    filePath = texture->filePath;
    config   = texture->config;
    latestTexture = texture;
    b_isDirty = false;
  }
private:
  std::shared_ptr<TextureData> mUnknownTexture;
};
