#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include <glad/glad.h>

#include "TextureData.h"

class TextureCreator {
  std::string filePath;
  TextureConfig config;
  // what the fuck does 'need action' mean to you? what it needs is
  // some courtesy to put some thought into the final name
  bool b_isDirty = false;

public:
  void renderImGui(const char* id);
  bool isDirty() const { return b_isDirty; }
  std::shared_ptr<TextureData> create();
};
