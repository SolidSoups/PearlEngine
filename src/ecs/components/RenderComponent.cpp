#include "RenderComponent.h"
#include "AssetDescriptor.h"
#include "AssetSystem.h"
#include "Logger.h"
#include "ResourceSystem.h"
#include "ServiceLocator.h"

void RenderComponent::SetMeshFromAsset(const pe::AssetDescriptor &desc) {
  if (desc.type != "Mesh_Asset") {
    LOG_ERROR << "Incompatible descriptor for conversion to mesh";
    return;
  }

  auto asset = ServiceLocator::Get<pe::AssetSystem>().LoadAsset(&desc);
  if (!asset)
    return;

  IResource *resource =
      ServiceLocator::Get<ResourceSystem>().LoadAsset(asset.get());
  if (!resource)
    return;

  Mesh* mesh = dynamic_cast<Mesh *>(resource);
  if (!mesh) {
    LOG_ERROR << "Resource is not mesh"; return;
  }

  meshHandle = ServiceLocator::Get<ResourceSystem>().Create(mesh);  
}


void RenderComponent::SetMaterialFromTextureAsset(const pe::AssetDescriptor& desc){
  LOG_INFO << "SetMaterial: "
    << "Stem: " << desc.stem << "\n"
    << "Type: " << desc.type << "\n"
    << "Extension: " << desc.extension << "\n"
    << "LocalPath: " << desc.localPath;
  // create a texture handle from the asset
  // set the unfiform for the texture
  auto asset = ServiceLocator::Get<pe::AssetSystem>().LoadAsset(&desc);
  if(!asset){
    LOG_ERROR << "Failed to load asset";
    return;
  }
  Texture_Asset* texAsset = dynamic_cast<Texture_Asset*>(asset.get());
  if(!texAsset){
    LOG_ERROR << "Dynamic cast faild: IAsset* -> Texture_Asset*";
    return;
  }
  
  auto& resourceSystem = ServiceLocator::Get<ResourceSystem>();
  TextureDataHandle texHandle = resourceSystem.LoadAsset<TextureData>(texAsset);
  
  MaterialSetTexture(&resourceSystem, materialHandle, "mainTexture", texHandle);
}
