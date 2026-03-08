#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include <glad/glad.h>

#include "TextureData.h"

class TextureCreator {
  std::shared_ptr<TextureData> latestTexture;
  bool b_isDirty = false;
  TextureConfig config;
  std::string filePath;

public:
  void renderImGui(const char* id);
  bool isDirty() const { return b_isDirty; }
  std::shared_ptr<TextureData> create();
};
