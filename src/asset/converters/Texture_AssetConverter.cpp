#include "Texture_AssetConverter.h"

#include <cstring>
#include <stb_image.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Logger.h"
#include "Texture_Asset.h"

bool Texture_AssetConverter::LoadTexture(
    const char *filePath, uint32_t &outWidth, uint32_t& outHeight,
    uint32_t& outChannels, std::vector<unsigned char> &outPixelData) {
  // set flip flag (global, uses OpenGL coordinates)
  stbi_set_flip_vertically_on_load(true);

  // load image using stbi
  int width = -1;
  int height = -1;
  int channels = -1;
  unsigned char *data = stbi_load(filePath, &width, &height, &channels, 0);

  // check if data couldn't be loaded
  if (!data) {
    LOG_ERROR << "stbi_load return NULL:\n"
              << "Reason: " << stbi_failure_reason();
    return false;
  }

  // add check on size
  if (width > 1024 || height > 1024) {
    LOG_WARNING << "Image size " << width << "x" << height
                << " is exceeding. OpenGL may not be able to handle such "
                   "large textures";
  }

  outWidth = width;
  outHeight = height;
  outChannels = channels;

  // Calculate total size on data
  size_t dataSize = width * height * channels;
  outPixelData.resize(dataSize);
  std::memcpy(outPixelData.data(), data, dataSize);

  // free the stbi data (we've copied it to the vector)
  stbi_image_free(data);

  return true;
}


std::unique_ptr<IAsset> Texture_AssetConverter::ConvertToAsset(const pe::FileDescriptor* file) {
  std::vector<unsigned char> temp_pixelData;
  uint32_t temp_width, temp_height, temp_channels;
  if(!LoadTexture(file->localPath.c_str(), temp_width, temp_height, temp_channels, temp_pixelData)){
    return nullptr;
  }
  
  auto asset = std::make_unique<Texture_Asset>(temp_pixelData, temp_width, temp_height, temp_channels);
  return asset;
}
std::unique_ptr<IAsset> Texture_AssetConverter::CreateEmptyAsset() {
  auto asset = std::make_unique<Texture_Asset>();
  return asset;
}
