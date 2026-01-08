#include "RenderComponent.h"
#include "AssetDescriptor.h"
#include "AssetSystem.h"
#include "IResource.h"
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
