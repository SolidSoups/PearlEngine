#include "Mesh_ResourceConverter.h"

IResource *Mesh_ResourceConverter::ConvertToResource(IAsset *asset) {
    // down cast to mesh
    Mesh_Asset *meshAsset = dynamic_cast<Mesh_Asset *>(asset);
    if (!meshAsset) {
        LOG_ERROR << "IAsset unable to be downcast to Mesh_Asset";
        return nullptr;
    }

    IResource *newMesh = new Mesh(meshAsset->vertices, meshAsset->indices);
    return newMesh;
};
