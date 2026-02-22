#include "ScriptSystem.h"
#include "ScriptComponent.h"
#include "ScriptEngine.h"

void ScriptSystem::Init(ecs::Coordinator* coord, ScriptEngine* engine){
  mCoordinator = coord;
  mEngine = engine;
}

// TOO MUCH FUCKING STATE!?
void ScriptSystem::OnUpdate(){
  for(auto entity : mEntities){
    // get all components with a script path
    ScriptComponent& sc = mCoordinator->GetComponent<ScriptComponent>(entity);
    std::string& path = sc.scriptPath;

    // log state
    // LOG_INFO << "Script State:\n"
    //   << "Path: " << path << "\n"
    //   << "Env: " << sc.scriptEnv.valid() << "\n"
    //   << "Enabled: " << sc.enabled << "\n"
    //   << "Loaded: " << sc.loaded << "\n"
    //   << "Reload requested: " << sc.needsReload << "\n"
    //   << "Has Error: " << sc.hasError;

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
  for(auto entity : mEntities){
    ScriptComponent& sc = mCoordinator->GetComponent<ScriptComponent>(entity);
    if(sc.loaded){
      mEngine->RunOnDestroy(entity, sc);
    }
  }
}
