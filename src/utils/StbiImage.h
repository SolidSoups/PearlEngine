#pragma once

#include <stb_image.h>

struct StbiImage {
  unsigned char *data;
  int width = -1, height = -1, channels = -1;

  StbiImage() = default;
  explicit StbiImage(const char *path, int forceChannels = 0) {
    stbi_set_flip_vertically_on_load(true);

    // load image using stbi
    data = stbi_load(path, &width, &height, &channels, forceChannels);
    
    // double check load
    if(!data){
      LOG_ERROR << "stbi_load returned null: "
        << stbi_failure_reason();
      return;
    }
  }
  ~StbiImage() {
    if (data)
      stbi_image_free(data);
  }

  // no copy
  StbiImage(const StbiImage &) = delete;
  StbiImage &operator=(const StbiImage &) = delete;
  // move ok
  StbiImage(StbiImage &&other) noexcept
      : data(other.data), width(other.width), height(other.height),
        channels(other.channels) {
    other.data = nullptr;
  }
  StbiImage &operator=(StbiImage &&other) noexcept {
    if (this != &other) {
      if (data)
        stbi_image_free(data);
      data = other.data;
      width = other.width;
      height = other.height;
      channels = other.channels;
      other.data = nullptr;
    }
    return *this;
  }
};
