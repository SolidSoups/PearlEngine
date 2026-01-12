#pragma once

#include <memory>

struct TextureData;

class TextureLoader {
public:
  static std::shared_ptr<TextureData> load(const char* path, bool genMipmaps = true);
};
