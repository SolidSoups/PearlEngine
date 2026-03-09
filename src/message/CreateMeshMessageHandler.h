#pragma once

#include "IMessageHandler.h"
#include "CreateMeshMessage.h"
#include "ServiceLocator.h"
#include "MeshManager.h"
#include "Scene.h"
#include "RenderComponent.h"

class CreateMeshMessageHandler : public IMessageHandler {
public:
  void HandleMessage(const Message& msg) override{
    if(auto createMesh = msg.As<CreateMeshMessage>()){
      auto& meshManager = ServiceLocator::Get<MeshManager>();
      std::shared_ptr<Mesh> newMesh = meshManager.loadOBJ(createMesh->filePath.c_str());
      if(!newMesh) return;

      auto& coord = ServiceLocator::Get<Scene>().GetCoordinator();
      if(coord.HasComponent<RenderComponent>(createMesh->renderCompEntity)){
        auto& renderComp = coord.GetComponent<RenderComponent>(createMesh->renderCompEntity);
        renderComp.mesh = newMesh;
        LOG_INFO << "Succesfully created mesh through message handler!";
      }
    } 
  }
};
