#include "ScriptComponentEditor.h"

#include <imgui.h>

#include "Logger.h"
#include "UserGUI.h"

#include "ServiceLocator.h"
#include "TextureManager.h"

void ScriptComponentEditor::OnInit(){
  auto& texManager = ServiceLocator::Get<TextureManager>();
  m_CheckTex = texManager.load("assets/checker.png");
  m_CrossTex = texManager.load("assets/cross.png");
  LOG_INFO << "Initialized icon textures";
}

void ScriptComponentEditor::OnDrawComponent(void* component, ecs::Entity entity) {
  ScriptComponent* scriptCmp = static_cast<ScriptComponent*>(component);
  if(!scriptCmp)
    return;

  // Constants
  const float labelWidth = 180.f; 
  const char* file_path = scriptCmp->scriptPath.empty() ? "none" : scriptCmp->scriptPath.c_str();

  // write out state string
  std::string scriptState = (scriptCmp->loaded ? "Loaded" : "Unloaded");
  scriptState += (scriptCmp->enabled ? "  -  Enabled" : "  -  Disabled");
  scriptState += (scriptCmp->hasError ? "  -  Failed" : "  -  Valid");

  // calculate spacing so text is centered
  float spacing = ImGui::GetStyle().ItemSpacing.x * 2;
  float totalWidth = ImGui::CalcTextSize(scriptState.c_str()).x + spacing;
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - totalWidth) * 0.5);

  // draw state text
  ImGui::Text("%s", scriptState.c_str());

  ImGui::Dummy(ImVec2(0, 5));

  // Script header
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Script");
  ImGui::SameLine();

  // Calculate input text length with enough space for the next button
  float buttonWidth = ImGui::CalcTextSize("...").x + ImGui::GetStyle().FramePadding.x * 2;
  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - buttonWidth - ImGui::GetStyle().ItemSpacing.x*2);


  // Draw read-only input box using the copied file path 
  char buf[256];
  strncpy(buf, file_path, sizeof(buf));
  ImGui::InputText("##scriptpath", buf, sizeof(buf), ImGuiInputTextFlags_ReadOnly);
  ImGui::SameLine();

  // Draw a button that let's the user pick the script to use
  if(ImGui::Button("...")){
    UserGUI::StartFilePopup([scriptCmp](const std::string& file){
      scriptCmp->scriptPath = file;
    }, {".lua"});
  }

  ImGui::Dummy(ImVec2(0, 5));


  // button to enable/disable component environment
  std::string triggerButtonText = (scriptCmp->enabled ? "Disable" : "Enable"); 
  if(ImGui::Button(triggerButtonText.c_str())){
    scriptCmp->enabled = !scriptCmp->enabled;
  }
  ImGui::SameLine();

  // Button to reload the script
  if(ImGui::Button("Reload Script")){
    scriptCmp->needsReload = true; 
  }
  ImGui::SameLine();

  // icon for failed/loaded state
  if(scriptCmp->hasError){
    ImGui::Image((ImTextureID)(uintptr_t)m_CrossTex->id, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));
  }
  else if(scriptCmp->loaded){
    ImGui::Image((ImTextureID)(uintptr_t)m_CheckTex->id, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));
  }
  else{
    ImGui::NewLine();
  }
}
