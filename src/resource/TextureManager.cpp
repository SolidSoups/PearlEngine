#include "TextureManager.h"

#include <memory>

#include <stb_image.h>

#include "TextureData.h"
#include "Logger.h"

std::shared_ptr<TextureData> TextureManager::load(const char* path, bool genMipMaps){
  auto it = m_Cache.find(path);
  if(it != m_Cache.end()){
    // texture is cached
    return it->second;
  }


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


  // cache texture
  m_Cache[path] = std::make_shared<TextureData>(data, width, height, channels, genMipMaps);

  // free image data (we don't need it anymore)
  stbi_image_free(data);

  return m_Cache[path];
}
