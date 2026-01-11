#include "TextureLoader.h"

#include <memory>

#include <stb_image.h>

#include "TextureData.h"

std::shared_ptr<TextureData> TextureLoader::load(const char* path, bool genMipMaps){
  // set flip flag (global, uses OpenGL coordinates)
  stbi_set_flip_vertically_on_load(true);

  // load image using stbi
  int width = -1;
  int height = -1;
  int channels = -1;
  unsigned char *data =
      stbi_load(path, &width, &height, &channels, 0);

  // check if data couldn't be loaded
  if (!data) {
    LOG_ERROR << "stbi_load return NULL:\n"
              << "Reason: " << stbi_failure_reason();
    return {};
  }

  auto texture = std::make_shared<TextureData>(data, width, height, channels, genMipMaps);
  stbi_image_free(data);
  return texture;
}
