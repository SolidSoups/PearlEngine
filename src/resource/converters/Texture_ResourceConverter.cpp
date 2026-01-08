#include "Texture_ResourceConverter.h"
#include "IResource.h"
#include "Texture_Asset.h"
#include "TextureData.h"
#include "Logger.h"

IResource *Texture_ResourceConverter::ConvertToResource(IAsset *asset) {
  // down cast to texture
  if (Texture_Asset *texAsset = dynamic_cast<Texture_Asset *>(asset)) {
    IResource *newTexture =
        new TextureData(texAsset->pixelData, texAsset->width, texAsset->height,
                        texAsset->channels, false);
    return newTexture;
  } else {
    LOG_ERROR << "IAsset unable to be downcast to Texture_Asset";
    return nullptr;
  }
}
