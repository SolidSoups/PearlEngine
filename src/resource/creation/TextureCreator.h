#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include <glad/glad.h>

#include "TextureData.h"

class TextureCreator {
  std::string filePath;
  TextureConfig config;
  bool b_isDirty = false;

public:
  void renderImGui(const char* id);
  bool isDirty() const { return b_isDirty; }
  std::shared_ptr<TextureData> create();
};
