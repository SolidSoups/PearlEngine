#include "ScriptSystem.h"
#include "ScriptComponent.h"
#include "ScriptEngine.h"

#include "ecs_system_impl.h"

void ScriptSystem::Init(ecs::Coordinator* coord, ScriptEngine* engine){
  mCoordinator = coord;
  mEngine = engine;
}

void ScriptSystem::OnCreateScene(){
  for(auto entity : Entities){
    auto& sc = Get<ScriptComponent>(entity);

    // skip all invalid or disabled script components
    if(sc.scriptPath.empty() or !sc.enabled) continue;

    // load component
    sc.loaded = mEngine->RunOnCreate(entity, sc);
  }
}

void ScriptSystem::OnUpdate(){
  for(auto entity : Entities){
    // get all components with a script path
    ScriptComponent& sc = Get<ScriptComponent>(entity);

    // skip invalid scriptcomponents
    if(sc.scriptPath.empty()) continue;

    // reload script. destroy current state, re-initialize next frame
    if(sc.needsReload){
      if(sc.loaded) mEngine->RunOnDestroy(entity, sc);
      mEngine->ResetScript(sc);
      sc.enabled = true; // force engine to reload (tmp)
      continue;
    }

    // teardown. script has error or was disabled
    if(sc.loaded && (!sc.enabled || sc.hasError)){
      mEngine->RunOnDestroy(entity, sc);
      continue;
    }

    if(!sc.enabled) continue;
    
    // initialize
    if(!sc.loaded){
      sc.loaded = mEngine->RunOnCreate(entity, sc);
      continue;
    }

    mEngine->RunOnUpdate(entity, sc);
  }

  // second pass
  for(auto entity : Entities){
    ScriptComponent& sc = Get<ScriptComponent>(entity);
    if(!sc.loaded || !sc.enabled || sc.hasError) continue;

    mEngine->RunOnLateUpdate(entity, sc);
  }
}

void ScriptSystem::OnDestroy(){
  for(auto entity : Entities){
    ScriptComponent& sc = Get<ScriptComponent>(entity);
    if(sc.loaded){
      mEngine->RunOnDestroy(entity, sc);
    }
  }
}

void ScriptSystem::DispatchOnCollisionEnter(ecs::Entity a, ecs::Entity b, const glm::vec3& normal, float penetration){
  if(auto* sc1 = TryGet<ScriptComponent>(a)){
    mEngine->RunOnCollisionEnter(a, *sc1, b, normal, penetration);        
  }
  if(auto* sc2 = TryGet<ScriptComponent>(b)){
    mEngine->RunOnCollisionEnter(b, *sc2, a, -normal, penetration);        
  }
}

