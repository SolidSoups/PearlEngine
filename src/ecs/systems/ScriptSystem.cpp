#include "ScriptSystem.h"
#include "ScriptComponent.h"
#include "ScriptEngine.h"

void ScriptSystem::Init(ecs::Coordinator* coord, ScriptEngine* engine){
  mCoordinator = coord;
  mEngine = engine;
}

void ScriptSystem::OnUpdate(){
  for(auto entity : mEntities){
    ScriptComponent& sc = mCoordinator->GetComponent<ScriptComponent>(entity);
    if(sc.scriptPath.empty()) continue;
    
    if(!sc.loaded){
      mEngine->RunOnCreate(entity, sc);
      sc.loaded = true;
    }
    mEngine->RunOnUpdate(entity, sc);
  }
}

void ScriptSystem::OnDestroy(){
  for(auto entity : mEntities){
    ScriptComponent& sc = mCoordinator->GetComponent<ScriptComponent>(entity);
    if(sc.loaded){
      mEngine->RunOnDestroy(entity, sc);
    }
  }
}
