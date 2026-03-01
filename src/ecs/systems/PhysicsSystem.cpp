#include "PhysicsSystem.h"

#include "ecs_system_impl.h"
#include "SphereColliderComponent.h"
#include "BoxColliderComponent.h"
#include "LineRenderer.h"
#include "SelectionWizard.h"

void PhysicsSystem::UpdatePhysics(){

}
void PhysicsSystem::DrawGizmos(){
  ecs::Entity selectedEntity = SelectionWizard::Get();
  if(selectedEntity == ecs::NULL_ENTITY) return;

  SphereColliderComponent sphereCmp;
  BoxColliderComponent boxComp;
  for(ecs::Entity entity : Entities){
    if(entity != selectedEntity) continue;

    if(TryGet<SphereColliderComponent>(entity, sphereCmp)){
      LineRenderer::DrawWireSphere(sphereCmp.position, sphereCmp.radius);
    }
    if(TryGet<BoxColliderComponent>(entity, boxComp)){
      LineRenderer::DrawWireBox(boxComp.center, boxComp.size);
    }
  }
}
