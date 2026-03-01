#include "PhysicsSystem.h"

#include "ecs_system_impl.h"
#include "SphereColliderComponent.h"
#include "LineRenderer.h"
#include "SelectionWizard.h"

void PhysicsSystem::UpdatePhysics(){

}
void PhysicsSystem::DrawGizmos(){
  ecs::Entity selectedEntity = SelectionWizard::Get();
  if(selectedEntity == ecs::NULL_ENTITY) return;

  for(ecs::Entity entity : Entities){
    if(entity != selectedEntity) continue;

    auto& sphereComp = Get<SphereColliderComponent>(entity);
    LineRenderer::DrawWireSphere(sphereComp.position, sphereComp.radius);
  }
}
