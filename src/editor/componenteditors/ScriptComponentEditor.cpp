#include "ScriptComponentEditor.h"

#include <imgui.h>

#include "Logger.h"
#include "UserGUI.h"

void ScriptComponentEditor::OnDrawComponent(void* component, ecs::Entity entity) {
  ScriptComponent* scriptCmp = static_cast<ScriptComponent*>(component);
  if(!scriptCmp)
    return;

  const float labelWidth = 180.f; 
  const char* file_path = scriptCmp->scriptPath.empty() ? "none" : scriptCmp->scriptPath.c_str();
  ImGui::Text("Script: %s", file_path);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-10);
  if(ImGui::Button("Set script")){
    UserGUI::StartFilePopup([scriptCmp](const std::string& file){
      LOG_INFO << "Loading file " << file; 
      scriptCmp->scriptPath = file;
    }, {".lua"});
  }

  std::string loaded = scriptCmp->loaded ? "Loaded" : "Unloaded";
  ImGui::Text("Is Loaded: %s", loaded.c_str());
}
