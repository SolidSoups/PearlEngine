#include "ScriptSystem.h"
#include "ScriptComponent.h"
#include "ScriptEngine.h"

#include "ecs_system_impl.h"

void ScriptSystem::Init(ecs::Coordinator* coord, ScriptEngine* engine){
  mCoordinator = coord;
  mEngine = engine;
}

void ScriptSystem::OnUpdate(){
  for(auto entity : Entities){
    // get all components with a script path
    ScriptComponent& sc = Get<ScriptComponent>(entity);
    std::string& path = sc.scriptPath;

    if(path.empty()) continue;

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
}

void ScriptSystem::OnDestroy(){
  for(auto entity : Entities){
    ScriptComponent& sc = mCoordinator->GetComponent<ScriptComponent>(entity);
    if(sc.loaded){
      mEngine->RunOnDestroy(entity, sc);
    }
  }
}
